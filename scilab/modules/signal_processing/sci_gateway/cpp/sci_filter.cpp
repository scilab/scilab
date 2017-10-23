/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2017 - ESI - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*

Copyright (C) 1996-2017 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Octave is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

// Based on Tony Richardson's filter.m.
//
// Originally translated to C++ by KH (Kurt.Hornik@wu-wien.ac.at)
// with help from Fritz Leisch and Andreas Weingessel on Oct 20, 1994.
//
// Rewritten to use templates to handle both real and complex cases by
// jwe, Wed Nov  1 19:15:29 1995.

#include "signal_gw.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "function.hxx"
#include "overload.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
}

const char fname[] = "filter";

static void clean_filter(types::Double* a, bool alloc_a, types::Double* b, bool alloc_b)
{
    if (alloc_a)
    {
        a->killMe();
    }

    if (alloc_b)
    {
        b->killMe();
    }
}

static types::Double* filter(types::Double* b, types::Double* a, types::Double* x, types::Double* si)
{
    bool alloc_a = false;
    bool alloc_b = false;

    int size_a = a->getSize();
    int size_b = b->getSize();

    int len = std::max(size_a, size_b);

    int dims[2] = {1, len};

    //uniformize
    types::Double* new_a = a->resize(dims, 2)->getAs<types::Double>();
    if (new_a != a)
    {
        alloc_a = true;
    }

    types::Double* new_b = b->resize(dims, 2)->getAs<types::Double>();
    if (new_b != b)
    {
        alloc_b = true;
    }

    double* pa = new_a->get();
    double* pb = new_b->get();
    double* px = x->get();

    double norm = pa[0];

    if (norm == 0)
    {
        clean_filter(new_a, alloc_a, new_b, alloc_b);
        Scierror(999, _("%s: Wrong value for input argument #%d: First element must not be %s.\n"), fname, 2, "0");
        return nullptr;
    }

    int size_x = x->getSize();

    int rows_si = si->getRows();

    if (rows_si != len - 1)
    {
        clean_filter(new_a, alloc_a, new_b, alloc_b);
        Scierror(999, "%s: first dimension of SI must be of length max (length (a), length (b)) - 1 (%d) (%d)", fname, rows_si, size_x);
        return nullptr;
    }

    if (size_x == 0)
    {
        clean_filter(new_a, alloc_a, new_b, alloc_b);
        return x;
    }

    if (norm != 1)
    {
        if (alloc_a == false)
        {
            new_a = new_a->clone();
        }

        if (alloc_b == false)
        {
            new_b = new_b->clone();
        }

        pa = new_a->get();
        pb = new_b->get();
        alloc_a = true;
        alloc_b = true;

        for (int i = 0; i < len; ++i)
        {
            pa[i] /= norm;
            pb[i] /= norm;
        }
    }

    if (size_a <= 1 && rows_si <= 0)
    {
        types::Double* ret = x->clone();
        double* pret = ret->get();
        int s = ret->getSize();
        for (int i = 0; i < s; ++i)
        {
            pret[i] *= pb[0];
        }

        clean_filter(new_a, alloc_a, new_b, alloc_b);
        return ret;
    }

    types::Double* y = new types::Double(x->getRows(), x->getCols());

    double* py = y->get();
    double* psi = si->get();
    if (size_a > 1)
    {
        if (rows_si > 0)
        {
            for (int i = 0; i < size_x; ++i)
            {
                py[i] = psi[0] + pb[0] * px[i];
                for (int j = 0; j < rows_si - 1; ++j)
                {
                    psi[j] = psi[j + 1] - pa[j + 1] * py[i] + pb[j + 1] * px[i];
                }

                psi[rows_si - 1] = pb[rows_si] * px[i] - pa[rows_si] * py[i];
            }
        }
        else
        {
            for (int i = 0; i < size_x; ++i)
            {
                py[i] = psi[0] + pb[0] * px[i];
                psi[0] = pb[rows_si] * px[i] - pa[rows_si] * py[i];
            }

        }
    }
    else
    {
        for (int i = 0; i < size_x; ++i)
        {
            py[i] = psi[0] + pb[0] * px[i];

            if (rows_si > 1)
            {
                for (int j = 0; j < rows_si - 1; j++)
                {
                    psi[j] = psi[j + 1] + pb[j + 1] * px[i];
                }

                psi[rows_si - 1] = pb[rows_si] * px[i];
            }
            else
            {
                psi[0] = pb[1] * px[i];
            }
        }
    }


    clean_filter(new_a, alloc_a, new_b, alloc_b);
    return y;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_filter(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRhs = (int)in.size();

    bool alloc_si = false;
    types::Double* b = nullptr;
    types::Double* a = nullptr;
    types::Double* x = nullptr;
    types::Double* si = nullptr;

    if (iRhs < 3 || iRhs > 4)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 3, 4);
        return types::Function::Error;
    }

    types::InternalType::ScilabId type_b = in[0]->getId();
    types::InternalType::ScilabId type_a = in[1]->getId();
    types::InternalType::ScilabId type_x = in[2]->getId();
    types::InternalType::ScilabId type_si = types::InternalType::IdDouble;

    if (iRhs > 3)
    {
        type_si = in[3]->getId();
    }

    if (type_b  != types::InternalType::IdDouble && type_b  != types::InternalType::IdScalarDouble ||
        type_a  != types::InternalType::IdDouble && type_a  != types::InternalType::IdScalarDouble ||
        type_x  != types::InternalType::IdDouble && type_x  != types::InternalType::IdScalarDouble ||
        type_si != types::InternalType::IdDouble && type_si != types::InternalType::IdScalarDouble)
    {
        return Overload::call(L"%_filter", in, _iRetCount, out);
    }

    b = in[0]->getAs<types::Double>();
    a = in[1]->getAs<types::Double>();
    x = in[2]->getAs<types::Double>();

    if (iRhs > 3)
    {
        si = in[3]->getAs<types::Double>();
    }
    else
    {
        alloc_si = true;
        int size_a = a->getSize();
        int size_b = b->getSize();
        int len = std::max(size_a, size_b) - 1;
        int leadDim = x->getRows() == 1 ? 1 : 0;

        int si_dims[2] = {x->getRows(), x->getCols()};
        for (int i = leadDim; i > 0; i--)
        {
            si_dims[i] = si_dims[i - 1];
        }

        si_dims[0] = len;

        si = new types::Double(2, si_dims);
        si->setZeros();
    }

    if (b->isVector() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (a->isVector() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2);
        return types::Function::Error;
    }

    if (x->isVector() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3);
        return types::Function::Error;
    }

    if (iRhs > 3 && si->isVector() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 4);
        return types::Function::Error;
    }

    types::Double* ret = filter(b, a, x, si);
    if (_iRetCount != 2 && alloc_si)
    {
        si->killMe();
    }

    if (ret == nullptr)
    {
        return types::Function::Error;
    }

    out.push_back(ret);
    if (_iRetCount == 2)
    {
        out.push_back(si);
    }
    return types::Function::OK;
}