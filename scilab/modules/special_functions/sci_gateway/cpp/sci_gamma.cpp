/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "special_functions_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
    extern double C2F(dgammacody)(double*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_gamma(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument: %d expected.\n"), "gamma", 1);
        return types::Function::Error;
    }

    if (in[0]->isList() || in[0]->isTList() || in[0]->isMList())
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_gamma";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A matrix expected.\n"), "gamma", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    types::Double* pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isComplex())
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "gamma", 1);
        return types::Function::Error;
    }

    if (pDblIn->getDims() > 2)
    {
        return Overload::call(L"%hm_gamma", in, _iRetCount, out);
    }

    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());

    double* pDblValIn  = pDblIn->getReal();
    double* pDblValOut = pDblOut->getReal();

    /***** perform operation *****/
    for (int i = 0; i < pDblIn->getSize(); i++)
    {
        pDblValOut[i] = C2F(dgammacody)(pDblValIn + i);
    }

    /***** return data *****/
    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
