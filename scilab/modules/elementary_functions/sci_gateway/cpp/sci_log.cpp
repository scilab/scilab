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
#include "sciprint.h"
#include "localization.h"
#include "elem_common.h"
#include "log.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_log(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iAlert = 1;

    if (in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"log", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"log", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_log";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();
    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    if (pDblIn->isComplex())
    {
        for (int i = 0; i < pDblIn->getSize(); i++)
        {
            //If the value is less than precision (eps).
            if (iAlert && pDblIn->get(i) == 0 && pDblIn->getImg(i) == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    ScierrorW(999, _W("%ls: Wrong value for input argument #%d : Singularity of the function.\n"), L"log", 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprintW(_W("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), L"log", 1);
                    }
                }

                iAlert = 0;
            }

            wlog(pDblIn->get(i), pDblIn->getImg(i), pDblOut->get() + i, pDblOut->getImg() + i);
        }
    }
    else
    {
        bool bIsLessZero = false;
        for (int i = 0; i < pDblIn->getSize(); i++)
        {
            if (iAlert && pDblIn->get(i) == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    ScierrorW(999, _W("%ls: Wrong value for input argument #%d : Singularity of the function.\n"), L"log", 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprintW(_W("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), L"log", 1);
                    }
                }
                iAlert = 0;
            }
            else if (pDblIn->get(i) < 0)
            {
                bIsLessZero = true;
            }
        }

        if (bIsLessZero)
        {
            pDblOut->setComplex(true);
            for (int i = 0; i < pDblIn->getSize(); i++)
            {
                wlog(pDblIn->get(i), 0, pDblOut->get() + i, pDblOut->getImg() + i);
            }
        }
        else
        {
            for (int i = 0; i < pDblIn->getSize(); i++)
            {
                pDblOut->set(i, dlogs(pDblIn->get(i)));
            }
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
