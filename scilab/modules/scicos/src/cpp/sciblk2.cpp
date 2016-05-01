/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <vector>
#include <algorithm>
#include <cstring>

#include "var2vec.hxx"
#include "vec2var.hxx"

#include "internal.hxx"
#include "callable.hxx"
#include "list.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sciblk2.h"
#include "scicos.h" /* set_block_error() */
}

static double toDouble(const int i)
{
    return static_cast<double>(i);
}

static void setErrAndFree(const int flag, types::typed_list out)
{
    set_block_error(flag);
    for (size_t i = 0; i < out.size(); ++i)
    {
        out[i]->killMe();
    }
}

/*--------------------------------------------------------------------------*/
void sciblk2(int* flag, int* nevprt, double* t, double xd[], double x[], int* nx, double z[], int* nz, double tvec[], int* ntvec, double rpar[], int* nrpar,
             int ipar[], int* nipar, double* inptr[], int insz[], int* nin, double* outptr[], int outsz[], int* nout, void* scsptr)
{
    types::typed_list in(8), out;

    types::Double* Flag = new types::Double(*flag);
    in[0] = Flag;

    types::Double* Nevprt = new types::Double(*nevprt);
    in[1] = Nevprt;

    types::Double* T = new types::Double(*t);
    in[2] = T;

    types::Double* X = new types::Double(*nx, 1);
    memcpy(X->get(), x, *nx * sizeof(double));
    in[3] = X;

    types::InternalType* Z;
    if (*nz == 0)
    {
        Z = types::Double::Empty();
    }
    else
    {
        if (!vec2var(std::vector<double>(z, z + *nz), Z))
        {
            setErrAndFree(-1, out);
            delete in[0];
            delete in[1];
            delete in[2];
            delete in[3];
            return;
        }
        if (!Z->isDouble())
        {
            setErrAndFree(-1, out);
            delete in[0];
            delete in[1];
            delete in[2];
            delete in[3];
            return;
        }
        //types::Double* Z = new types::Double(*nz, 1);
        //memcpy(Z->get(), z, *nz * sizeof(double));
    }
    in[4] = Z->getAs<types::Double>();

    types::Double* Rpar = new types::Double(*nrpar, 1);
    memcpy(Rpar->get(), rpar, *nrpar * sizeof(double));
    in[5] = Rpar;

    // Treating 'ipar' differently because it is an int tab, unlike the other double ones
    types::Double* Ipar = new types::Double(*nipar, 1);
    std::transform(ipar, ipar + *nipar, Ipar->get(), toDouble);
    in[6] = Ipar;

    types::List* Nin = new types::List();
    for (int i = 0; i < *nin; ++i)
    {
        int nu = insz[i];
        int nu2 = insz[*nin + i];
        types::Double* U = new types::Double(nu, nu2);
        memcpy(U->get(), inptr[i], nu * nu2 * sizeof(double));
        Nin->append(U);
    }
    in[7] = Nin;

    /***********************
    * Call Scilab function *
    ***********************/
    types::Callable* pCall = static_cast<types::Callable*>(scsptr);

    try
    {
        types::optional_list opt;
        if (pCall->call(in, opt, 5, out) != types::Function::OK)
        {
            setErrAndFree(-1, out);
            return;
        }

        if (out.size() != 5)
        {
            setErrAndFree(-1, out);
            return;
        }
    }
    catch (const ast::InternalError& /*ie*/)
    {
        setErrAndFree(-1, out);
        return;
    }

    switch (*flag)
    {
        case 1 :
        case 2 :
        case 4 :
        case 5 :
        case 6 :
        {
            if (!out[2]->isDouble())
            {
                setErrAndFree(-1, out);
                return;
            }
            std::vector<double> Zout;
            if (!var2vec(out[2], Zout))
            {
                setErrAndFree(-1, out);
                return;
            }
            memcpy(z, &Zout[0], *nz * sizeof(double));

            if (!out[3]->isDouble())
            {
                setErrAndFree(-1, out);
                return;
            }
            types::Double* Xout = out[3]->getAs<types::Double>();
            memcpy(x, Xout->get(), *nx * sizeof(double));

            if (*flag == 1 || *flag == 6)
            {
                if (*nout != 0)
                {
                    if (!out[4]->isList())
                    {
                        setErrAndFree(-1, out);
                        return;
                    }
                    types::List* Yout = out[4]->getAs<types::List>();
                    if (Yout->getSize() < *nout)
                    {
                        // Consider that 'outptr' has not been defined in the macro: do not update the current 'outptr'
                        break;
                    }
                    for (int k = *nout - 1; k >= 0; --k)
                    {
                        if (!Yout->get(k)->isDouble())
                        {
                            setErrAndFree(-1, out);
                            return;
                        }
                        types::Double* KthElement = Yout->get(k)->getAs<types::Double>();
                        double* y = (double*)outptr[k];
                        int ny = outsz[k];
                        int ny2 = 1;
                        if (*flag == 1)
                        {
                            ny2 = outsz[*nout + k];
                        }
                        if (KthElement->getSize() != ny * ny2)
                        {
                            // At initialization (flag 6), the 'y' returned by the macro is not necessarily properly initialized.
                            // In this case, do nothing to avoid copying corrupt data
                            break;
                        }
                        memcpy(y, KthElement->get(), ny * ny2 * sizeof(double));
                    }
                }
            }
            break;
        }

        case 0 :
            /*  x'  computation */
        {
            if (!out[0]->isDouble())
            {
                setErrAndFree(-1, out);
                return;
            }
            types::Double* XDout = out[0]->getAs<types::Double>();
            memcpy(xd, XDout->get(), *nx * sizeof(double));
            break;
        }

        case 3 :
        {
            if (!out[1]->isDouble())
            {
                setErrAndFree(-1, out);
                return;
            }
            types::Double* Tout = out[1]->getAs<types::Double>();
            memcpy(tvec, Tout->get(), *ntvec * sizeof(double));
            break;
        }

        default :
        {
            setErrAndFree(-1, out);
            return;
        }
    }

    for (size_t i = 0; i < out.size(); ++i)
    {
        out[i]->killMe();
    }
}
