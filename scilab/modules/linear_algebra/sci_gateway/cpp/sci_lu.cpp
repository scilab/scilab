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
#include "lu.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_lu(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    types::Double* pDblL    = NULL;
    types::Double* pDblU    = NULL;
    types::Double* pDblE    = NULL;
    double* pdL             = NULL;
    double* pdU             = NULL;
    double* pData           = NULL;
    int iMinRowsCols        = 0;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "lu", 1);
        return types::Function::Error;
    }

    if (_iRetCount < 2 || _iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lu", 2, 3);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_lu";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (pDbl->isComplex())
    {
        pData = (double*)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        if (!pData)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), "lu");
            return types::Function::Error;
        }
    }
    else
    {
        pData = pDbl->getReal();
    }

    if ((pDbl->getCols() == 0) || (pDbl->getRows() == 0))
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
        if (_iRetCount == 3)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if ((pDbl->getRows() == -1) || (pDbl->getCols() == -1)) // Rhs(1)=k*eye() => Lhs(1)=eye()
    {
        // Lhs(2)=k*eye(), Lhs(3)=eye()
        pDblL = new types::Double(-1, -1, pDbl->isComplex());
        pDblL->set(0, 1);
        out.push_back(pDblL);
        out.push_back(pDbl);
        if (_iRetCount == 3)
        {
            pDblE = new types::Double(-1, -1, pDbl->isComplex());
            pDblE->set(0, 1);
            out.push_back(pDblE);
        }
        return types::Function::OK;
    }

    iMinRowsCols = std::min(pDbl->getRows(), pDbl->getCols());
    pDblL = new types::Double(pDbl->getRows(), iMinRowsCols, pDbl->isComplex());
    pDblU = new types::Double(iMinRowsCols, pDbl->getCols(), pDbl->isComplex());

    if (pDbl->isComplex())
    {
        pdL = (double*) MALLOC(pDblL->getSize() * sizeof(doublecomplex));
        pdU = (double*) MALLOC(pDblU->getSize() * sizeof(doublecomplex));
    }
    else
    {
        pdL = pDblL->get();
        pdU = pDblU->get();
    }

    if (_iRetCount == 3)
    {
        pDblE = new types::Double(pDbl->getRows(), pDbl->getRows());
    }

    int iRet = iLuM(pData, pDbl->getRows(), pDbl->getCols(), pDbl->isComplex(), pdL, pdU, (pDblE ? pDblE->get() : NULL));

    if (iRet != 0)
    {
        Scierror(999, _("%s: LAPACK error n°%d.\n"), "lu", iRet);
        FREE((doublecomplex*)pdL);
        FREE((doublecomplex*)pdU);
        delete pDblL;
        delete pDblU;
        delete pDblE;
        return types::Function::Error;
    }

    if (pDbl->isComplex())
    {
        vGetPointerFromDoubleComplex((doublecomplex*)pdL, pDblL->getSize(), pDblL->getReal(), pDblL->getImg());
        FREE((doublecomplex*)pdL);
        vGetPointerFromDoubleComplex((doublecomplex*)pdU, pDblU->getSize(), pDblU->getReal(), pDblU->getImg());
        FREE((doublecomplex*)pdU);
    }

    if (pDbl->isComplex())
    {
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }

    out.push_back(pDblL);
    out.push_back(pDblU);
    if (_iRetCount == 3)
    {
        out.push_back(pDblE);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

