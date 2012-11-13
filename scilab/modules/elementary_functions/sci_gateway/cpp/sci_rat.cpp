/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <algorithm>
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"

    extern void C2F(rat)(double*, double*, int*, int*, int*);
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_rat(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblN    = NULL; // numerator
    types::Double* pDblD    = NULL; // denominator
    types::Double* pDblOut  = NULL; // if _iRetCount == 1 the result is N/D

    double dblTol  = 1.e-6;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "rat", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "rat", 1, 2);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_rat";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->getDims() > 2)
    {
        return Overload::call(L"%hm_rat", in, _iRetCount, out, new ExecVisitor());
    }

    if (pDblIn->isComplex())
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_rat";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A constant scalar expected.\n"), "rat", 2);
            return types::Function::Error;
        }

        dblTol = in[1]->getAs<types::Double>()->get(0);
    }

    /***** perform operation and set result *****/
    double dblIn    = 0;
    int dblN        = 0;
    int dblD        = 0;
    int iFail       = 0;
    double dblRTol  = 0;

    // Make tolerance relative to the element with maximum absolute value
    for (int i = 0; i < pDblIn->getSize(); i++)
    {
        dblRTol = max(dblRTol, std::abs(pDblIn->get(i)));
    }

    if (dblRTol > 0)
    {
        dblTol = dblTol * dblRTol;
    }

    if (_iRetCount == 1)
    {
        pDblOut = new types::Double(pDblIn->getRows(), pDblIn->getCols());
        for (int i = 0; i < pDblIn->getSize(); i++)
        {
            dblIn = pDblIn->get(i);
            C2F(rat)(&dblIn, &dblTol, &dblN, &dblD, &iFail);
            if (iFail)
            {
                Scierror(999, _("%s: The tolerance is too large for the value %d.\n"), "rat", i);
                return types::Function::Error;
            }
            pDblOut->set(i, (double)dblN / (double)dblD);
        }

        out.push_back(pDblOut);
    }
    else // _iRetCount == 2
    {
        pDblN = new types::Double(pDblIn->getRows(), pDblIn->getCols());
        pDblD = new types::Double(pDblIn->getRows(), pDblIn->getCols());

        for (int i = 0; i < pDblIn->getSize(); i++)
        {
            dblIn = pDblIn->get(i);
            C2F(rat)(&dblIn, &dblTol, &dblN, &dblD, &iFail);
            if (iFail)
            {
                Scierror(999, _("%s: The tolerance is too large for the value %d.\n"), "rat", i);
                return types::Function::Error;
            }
            pDblN->set(i, dblN);
            pDblD->set(i, dblD);
        }

        out.push_back(pDblN);
        out.push_back(pDblD);
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
