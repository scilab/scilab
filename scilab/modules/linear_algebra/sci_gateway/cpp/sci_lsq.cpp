/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "lsq.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_lsq(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl[2]      = {NULL, NULL};
    types::Double* pDblResult   = NULL;
    double* pData[2]            = {NULL, NULL};
    double* pResult             = NULL;
    double* pdTol               = NULL;
    bool bComplexArgs           = false;
    int iRank                   = 0;
    double dblTol               = 0.0; 

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lsq", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lsq", 1, 2);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_lsq";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl[0] = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (in.size() <=  3)
    {
        if ((in[1]->isDouble() == false))
        {
            std::wstring wstFuncName = L"%" + in[1]->getShortTypeStr() + L"_lsq";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
        pDbl[1] = in[1]->getAs<types::Double>()->clone()->getAs<types::Double>();
    }

    if (in.size() == 3)
    {
        if ((in[2]->isDouble() == false) || (in[2]->getAs<types::Double>()->isComplex()) || (in[2]->getAs<types::Double>()->isScalar() == false))
        {
            Scierror(256, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "lsq", 3);
            return types::Function::Error;
        }

        dblTol = in[2]->getAs<types::Double>()->get(0);
        pdTol = &dblTol;
    }

    if (pDbl[0]->getRows() != pDbl[1]->getRows())
    {
        Scierror(265, _("%s: %s and %s must have equal number of rows.\n"), "lsq", "A", "B");
        return types::Function::Error;
    }

    if ((pDbl[0]->getCols() == 0) || (pDbl[1]->getCols() == 0))
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if (pDbl[0]->isComplex() || pDbl[1]->isComplex())
    {
        bComplexArgs = true;
    }
    for (int i = 0; i < 2; i++)
    {
        if (pDbl[i]->getCols() == -1)
        {
            Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "lsq", i + 1);
            return types::Function::Error;
        }

        if (bComplexArgs)
        {
            pData[i] = (double*)oGetDoubleComplexFromPointer(pDbl[i]->getReal(), pDbl[i]->getImg(), pDbl[i]->getSize());
            if (!pData[i])
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "lsq");
                return types::Function::Error;
            }
        }
        else
        {
            pData[i] = pDbl[i]->getReal();
        }
    }

    pDblResult = new types::Double(pDbl[0]->getCols(), pDbl[1]->getCols(), bComplexArgs);

    if (bComplexArgs)
    {
        pResult = (double*)MALLOC(pDbl[0]->getCols() * pDbl[1]->getCols() * sizeof(doublecomplex));
    }
    else
    {
        pResult = pDblResult->get();
    }

    int iRet = iLsqM(pData[0], pDbl[0]->getRows(), pDbl[0]->getCols(), pData[1], pDbl[1]->getCols(), bComplexArgs, pResult, pdTol, ((_iRetCount == 2) ? &iRank : NULL));

    if (iRet != 0)
    {
        if (iRet == -1)
        {
            Scierror(999, _("%s: Allocation failed.\n"),  "lsq");
        }
        else
        {
            Scierror(999, _("%s: LAPACK error n°%d.\n"),  "lsq", iRet);
        }
        return types::Function::Error;
    }

    if (bComplexArgs)
    {
        vGetPointerFromDoubleComplex((doublecomplex*)(pResult), pDblResult->getSize(), pDblResult->getReal(), pDblResult->getImg());
        vFreeDoubleComplexFromPointer((doublecomplex*)pResult);
        vFreeDoubleComplexFromPointer((doublecomplex*)pData[0]);
        vFreeDoubleComplexFromPointer((doublecomplex*)pData[1]);
    }

    out.push_back(pDblResult);
    if (_iRetCount == 2)
    {
        types::Double* pDblRank = new types::Double(1, 1);
        pDblRank->set(0, iRank);
        out.push_back(pDblRank);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

