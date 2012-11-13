/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "tan.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_tan(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "tan", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "tan", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_tan";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDblIn = in[0]->getAs<types::Double>();
    pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    if (pDblIn->isComplex())
    {
        for (int i = 0 ; i < pDblIn->getSize() ; i++)
        {
            ztans(pDblIn->get(i), pDblIn->getImg(i), pDblOut->get() + i, pDblOut->getImg() + i);
        }
    }
    else
    {
        for (int i = 0 ; i < pDblIn->getSize() ; i++)
        {
            pDblOut->set(i, dtans(pDblIn->get(i)));
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
