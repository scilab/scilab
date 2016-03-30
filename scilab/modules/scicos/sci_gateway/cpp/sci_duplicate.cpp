/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
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

#include <string>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

static const std::string funname = "duplicate";

static void duplicata(int n, double* v, double* w, double* ww, int& nw)
{
    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < static_cast<int>(w[i]); ++j)
        {
            ww[k] = v[i];
            k++;
        }
    }
    nw = k;
}

static void comp_size(double* v, int& nw, int n)
{
    nw = 0;
    for (int i = 0; i < n; ++i)
    {
        if (v[i] > 0)
        {
            nw += static_cast<int>(v[i]);
        }
    }
}

types::Function::ReturnValue sci_duplicate(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::Double* pIn1 = in[0]->getAs<types::Double>();
    if (pIn1->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }
    types::Double* pIn2 = in[1]->getAs<types::Double>();
    if (pIn2->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), funname.data(), 2);
        return types::Function::Error;
    }

    int m1, n1, n, m2, n2, m3, n3;

    m1 = pIn1->getRows();
    n1 = pIn1->getCols();
    n = m1 * n1;

    if (n == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    m2 = pIn2->getRows();
    n2 = pIn2->getCols();

    if (n != m2 * n2)
    {
        Scierror(999, _("%s: 1st and 2nd argument must have equal size\n"), funname.data());
        return types::Function::Error;
    }

    comp_size(pIn2->getReal(), n3, n);

    m3 = 1;
    double* d3;
    types::Double* pOut = new types::Double(n3, m3, &d3);
    duplicata(n, pIn1->getReal(), pIn2->getReal(), d3, n3);
    out.push_back(pOut);

    return types::Function::OK;
}
