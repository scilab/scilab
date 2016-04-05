/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include <cstring>
#include <cstdio>

#include "internal.hxx"
#include "callable.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "int.hxx"
#include "function.hxx"

extern "C"
{
#include "sciblk4.h"
#include "scicos.h"
#include "import.h"
}

#include "createblklist.hxx"

/*--------------------------------------------------------------------------*/
template <typename T>
bool sci2var(T* p, void* dest, const int row, const int col)
{
    const int size = p->getSize();
    typename T::type* srcR = p->get();

    if (row != p->getRows())
    {
        return false;
    }

    if (col != p->getCols())
    {
        return false;
    }

    if (p->isComplex())
    {
        typename T::type* srcI = p->getImg();
        if (dest == nullptr)
        {
            return false;
        }

        typename T::type* destR = (typename T::type*)dest;
        typename T::type* destI = destR + size;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
            destI[i] = srcI[i];
        }
    }
    else
    {
        if (dest == nullptr)
        {
            return false;
        }

        typename T::type* destR = (typename T::type*)dest;
        for (int i = 0; i < size; ++i)
        {
            destR[i] = srcR[i];
        }
    }

    return true;
}

/*--------------------------------------------------------------------------*/
static bool sci2var(types::InternalType* p, void* dest, const int desttype, const int row, const int col)
{
    switch (p->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            if (p->getAs<types::Double>()->isComplex() && desttype == SCSCOMPLEX_N)
            {
                return sci2var(p->getAs<types::Double>(), dest, row, col);
            }

            if (p->getAs<types::Double>()->isComplex() == false && desttype == SCSREAL_N)
            {
                return sci2var(p->getAs<types::Double>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt8:
        {
            if (desttype == SCSINT8_N)
            {
                return sci2var(p->getAs<types::Int8>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt16:
        {
            if (desttype == SCSINT16_N)
            {
                return sci2var(p->getAs<types::Int16>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabInt32:
        {
            if (desttype == SCSINT32_N)
            {
                return sci2var(p->getAs<types::Int32>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt8:
        {
            if (desttype == SCSUINT8_N)
            {
                return sci2var(p->getAs<types::UInt8>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt16:
        {
            if (desttype == SCSUINT16_N)
            {
                return sci2var(p->getAs<types::UInt16>(), dest, row, col);
            }
        }
        case types::InternalType::ScilabUInt32:
        {
            if (desttype == SCSUINT32_N)
            {
                return sci2var(p->getAs<types::UInt32>(), dest, row, col);
            }
        }
        default:
            return false;
    }

    return false;
}

/*--------------------------------------------------------------------------*/
static bool getDoubleArray(types::InternalType* p, double* dest)
{
    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        const int size = d->getSize();
        if (size == 0)
        {
            return true;
        }

        if (dest == nullptr)
        {
            return false;
        }

        memcpy(dest, d->get(), sizeof(double) * size);
        return true;
    }

    return false;
}

/*--------------------------------------------------------------------------*/
static bool getDoubleArrayAsInt(types::InternalType* p, int* dest)
{
    if (p == nullptr)
    {
        return false;
    }

    if (p->isDouble())
    {
        types::Double* d = p->getAs<types::Double>();
        const int size = d->getSize();
        if (size == 0)
        {
            return true;
        }

        double* dbl = d->get();
        for (int i = 0; i < size; ++i)
        {
            dest[i] = static_cast<int>(dbl[i]);
        }
        return true;
    }

    return false;
}

/*--------------------------------------------------------------------------*/
void sciblk4(scicos_block* Blocks, const int flag)
{
    int ierr = 0;
    /* Retrieve block number */
    const int kfun = get_block_number();

    /* Retrieve 'funtyp' by import structure */
    int* ptr = nullptr;
    int nv = 0, mv = 0;
    char buf[] = "funtyp";
    ierr = getscicosvarsfromimport(buf, (void**)&ptr, &nv, &mv);
    if (ierr == 0)
    {
        set_block_error(-1);
        return;
    }
    const int* const funtyp = (int *)ptr;

    types::typed_list in, out;
    types::optional_list opt;

    /*****************************
    * Create Scilab tlist Blocks *
    *****************************/
    types::InternalType* pIT = createblklist(Blocks, -1, funtyp[kfun - 1]);
    if (pIT == nullptr)
    {
        set_block_error(-1);
        return;
    }

    in.push_back(pIT);
    /* * flag * */
    in.push_back(new types::Double(flag));

    /***********************
    * Call Scilab function *
    ***********************/
    types::Callable* pCall = static_cast<types::Callable*>(Blocks->scsptr);

    try
    {
        if (pCall->call(in, opt, 1, out) != types::Function::OK)
        {
            set_block_error(-1);
            return;
        }

        if (out.size() != 1)
        {
            set_block_error(-1);
            return;
        }
    }
    catch (const ast::InternalError& /*ie*/)
    {
        set_block_error(-1);
        return;
    }

    pIT = out[0];
    if (pIT->isTList() == false)
    {
        set_block_error(-1);
        delete pIT;
        return;
    }

    types::TList* t = pIT->getAs<types::TList>();

    switch (flag)
    {
        /**************************
        * update continuous state
        **************************/
        case 0:
        {
            if (Blocks->nx != 0)
            {
                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }

                if ((funtyp[kfun - 1] == 10004) || (funtyp[kfun - 1] == 10005))
                {
                    /* 15 - res */
                    if (getDoubleArray(t->getField(L"res"), Blocks->res) == false)
                    {
                        t->killMe();
                        set_block_error(-1);
                        return;
                    }
                }
            }
            break;
        }
        /**********************
        * update output state
        **********************/
        case 1:
        {
            /* 21 - outptr */
            if (Blocks->nout > 0)
            {
                types::InternalType* pIT = t->getField(L"outptr");
                if (pIT && pIT->isList())
                {
                    types::List* lout = pIT->getAs<types::List>();
                    if (Blocks->nout == lout->getSize())
                    {
                        for (int i = 0; i < Blocks->nout; ++i)
                        {
                            //update data
                            int row = Blocks->outsz[i];
                            int col = Blocks->outsz[i + Blocks->nout];
                            int type = Blocks->outsz[i + Blocks->nout * 2];
                            if (sci2var(lout->get(i), Blocks->outptr[i], type, row, col) == false)
                            {
                                t->killMe();
                                set_block_error(-1);
                                return;
                            }
                        }
                    }
                }
            }
            break;
        }
        case 2:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            break;
        }

        /***************************
        * update event output state
        ***************************/
        case 3:
        {
            /* 23 - evout */
            if (getDoubleArray(t->getField(L"evout"), Blocks->evout) == false)
            {
                t->killMe();
                set_block_error(-1);
                return;
            }
            break;
        }
        /**********************
        * state initialisation
        **********************/
        case 4:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            break;
        }

        case 5:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            break;
        }

        /*****************************
        * output state initialisation
        *****************************/
        case 6:
        {
            /* 7 - z */
            if (Blocks[0].nz != 0)
            {
                if (getDoubleArray(t->getField(L"z"), Blocks->z) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            /* 11 - oz */
            //TODO : how to store object ?

            if (Blocks[0].nx != 0)
            {
                /* 13 - x */
                if (getDoubleArray(t->getField(L"x"), Blocks->x) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }

                /* 14 - xd */
                if (getDoubleArray(t->getField(L"xd"), Blocks->xd) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }

            /* 21 - outptr */
            if (Blocks->nout > 0)
            {
                types::InternalType* pIT = t->getField(L"outptr");
                if (pIT && pIT->isList())
                {
                    types::List* lout = pIT->getAs<types::List>();
                    if (Blocks->nout == lout->getSize())
                    {
                        for (int i = 0; i < Blocks->nout; ++i)
                        {
                            //update data
                            const int row = Blocks->outsz[i];
                            const int col = Blocks->outsz[i + Blocks->nout];
                            const int type = Blocks->outsz[i + Blocks->nout * 2];
                            if (sci2var(lout->get(i), Blocks->outptr[i], type, row, col) == false)
                            {
                                t->killMe();
                                set_block_error(-1);
                                return;
                            }
                        }
                    }
                }
            }
            break;
        }

        /*******************************************
        * define property of continuous time states
        * (algebraic or differential states)
        *******************************************/
        case 7:
        {
            if (Blocks[0].nx != 0)
            {
                /* 40 - xprop */
                if (getDoubleArrayAsInt(t->getField(L"xprop"), Blocks->xprop) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }
            break;
        }

        /****************************
        * zero crossing computation
        ****************************/
        case 9:
        {
            /* 33 - g */
            if (getDoubleArray(t->getField(L"g"), Blocks->g) == false)
            {
                t->killMe();
                set_block_error(-1);
                return;
            }

            if (get_phase_simulation() == 1)
            {
                /* 39 - mode */
                if (getDoubleArrayAsInt(t->getField(L"mode"), Blocks->mode) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }
            break;
        }
        /**********************
        * Jacobian computation
        **********************/
        case 10:
        {
            if ((funtyp[kfun - 1] == 10004) || (funtyp[kfun - 1] == 10005))
            {
                /* 15 - res */
                if (getDoubleArray(t->getField(L"res"), Blocks->res) == false)
                {
                    t->killMe();
                    set_block_error(-1);
                    return;
                }
            }
            break;
        }
    }

    t->killMe();
    return;
}
/*--------------------------------------------------------------------------*/
