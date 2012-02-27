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
#include "abs.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_atan(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblX   = NULL;
    types::Double* pDblY   = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"atan", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"atan", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_atan";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pDblX = in[0]->getAs<types::Double>();

    if (in.size() == 1)
    {
        if (pDblX->isComplex())
        {
            pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), true);
            for (int i = 0 ; i < pDblX->getSize() ; i++)
            {
                if (pDblX->get(i) == 0 && dabss(pDblX->getImg(i)) == 1)
                {
                    ScierrorW(999, _W("%ls: Wrong value for input argument #%d : Singularity of the fonction.\n"), L"atan", 1);
                    return types::Function::Error;
                }

                watan(pDblX->get(i), pDblX->getImg(i), pDblOut->get() + i, pDblOut->getImg() + i);
            }
        }
        else
        {
            pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), false);
            for (int i = 0 ; i < pDblX->getSize() ; i++)
            {
                pDblOut->set(i, datans(pDblX->get(i)));
            }
        }
    }
    else // in.size() == 2
    {
        pDblY = in[1]->getAs<types::Double>();

        if (pDblX->isComplex() || pDblY->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"atan", 2);
            return types::Function::Error;
        }

        if (pDblX->getSize() != pDblY->getSize())
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d and #%d: Same size expected.\n"), L"atan", 1, 2);
            return types::Function::Error;
        }

        pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), false);
        for (int i = 0 ; i < pDblX->getSize() ; i++)
        {
            pDblOut->set(i, datan2s(pDblX->get(i), pDblY->get(i)));
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
