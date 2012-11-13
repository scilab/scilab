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
#include "cos.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_acos(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "acos", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "acos", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_acos";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isComplex())
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);
        for (int i = 0 ; i < pDblIn->getSize() ; i++)
        {
            wacos(pDblIn->get(i), pDblIn->getImg(i), pDblOut->get() + i, pDblOut->getImg() + i);
        }
    }
    else
    {
        bool bOutSide = 0;
        //check if all variables are between [-1,1]
        for (int i = 0 ; i < pDblIn->getSize() ; i++)
        {
            if (fabs(pDblIn->get(i)) > 1)
            {
                bOutSide = 1;
                break;
            }
        }

        if (bOutSide) // Values outside [-1,1]
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                wacos(pDblIn->get(i), 0, pDblOut->get() + i, pDblOut->getImg() + i);
            }
        }
        else //all values are in [-1,1]
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), false);
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                pDblOut->set(i, dacoss(pDblIn->get(i)));
            }
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
