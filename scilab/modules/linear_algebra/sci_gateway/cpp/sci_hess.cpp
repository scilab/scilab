/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/

#include "linear_algebra_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "hess.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_hess(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    types::Double* pDblH    = NULL;
    double* pdH             = NULL;
    double* pData           = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "hess", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "hess", 1, 2);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_hess";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (pDbl->getRows() != pDbl->getCols())
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "hess", 1);
        return types::Function::Error;
    }

    if ((pDbl->getCols() == 0) || (pDbl->getRows() == 0))
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if (pDbl->getCols() == -1)
    {
        types::Double* pDblEyeMatrix = new types::Double(-1, -1);
        out.push_back(pDblEyeMatrix);
        return types::Function::Error;
    }

    if (pDbl->isComplex())
    {
        pData = (double *)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        if (!pData)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), "hess");
            return types::Function::Error;
        }
    }
    else
    {
        pData = pDbl->getReal();
    }

    if (_iRetCount == 2)
    {
        pDblH = new types::Double(pDbl->getRows(), pDbl->getCols(), pDbl->isComplex());
        if (pDbl->isComplex())
        {
            pdH = (double*)MALLOC(pDblH->getSize() * sizeof(doublecomplex));
            if (!pdH)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "hess");
                return types::Function::Error;
            }
        }
        else
        {
            pdH = pDblH->getReal();
        }
    }

    int iRet = iHessM(pData, pDbl->getCols(), pDbl->isComplex(), pdH);
    if (iRet != 0)
    {
        Scierror(999, _("%s: LAPACK error n°%d.\n"), "hess", iRet);
        return types::Function::Error;
    }

    if (pDbl->isComplex())
    {
        vGetPointerFromDoubleComplex((doublecomplex*)(pData), pDbl->getSize(), pDbl->getReal(), pDbl->getImg());
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);

        if (_iRetCount == 2)
        {
            vGetPointerFromDoubleComplex((doublecomplex*)(pdH), pDblH->getSize(), pDblH->getReal(), pDblH->getImg());
            vFreeDoubleComplexFromPointer((doublecomplex*)pdH);
        }
    }

    if (_iRetCount == 1)
    {
        out.push_back(pDbl);
    }
    else
    {
        out.push_back(pDblH);
        out.push_back(pDbl);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

