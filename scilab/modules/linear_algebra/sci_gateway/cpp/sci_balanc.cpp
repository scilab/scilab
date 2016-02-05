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
#include "balanc.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_balanc(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl[2]      = {NULL, NULL};
    types::Double* pDblRes[2]   = {NULL, NULL};
    double* pData[2]            = {NULL, NULL};
    bool bComplex               = false;

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "balanc", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount != 2 * in.size())
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "balanc", 2 * in.size());
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_balanc";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl[0] = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();
    if (pDbl[0]->isComplex())
    {
        bComplex = true;
    }

    if (in.size() == 2)
    {
        if ((in[1]->isDouble() == false))
        {
            std::wstring wstFuncName = L"%" + in[1]->getShortTypeStr() + L"_balanc";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        pDbl[1] = in[1]->getAs<types::Double>()->clone()->getAs<types::Double>();
        if (pDbl[1]->isComplex())
        {
            bComplex = true;
        }

        if (pDbl[0]->getRows() != pDbl[1]->getRows())
        {
            Scierror(999, _("%s: Arguments %d and %d must have equal dimensions.\n"), "balanc", 1, 2);
            return types::Function::Error;
        }
    }

    if (pDbl[0]->getCols() == 0)
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
        if (_iRetCount == 4)
        {
            out.push_back(types::Double::Empty());
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    if (pDbl[0]->getRows() != pDbl[0]->getCols())
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "balanc", 1);
        return types::Function::Error;
    }

    for (int i = 0; i < in.size(); i++)
    {
        if (bComplex)
        {
            pData[i] = (double*)oGetDoubleComplexFromPointer(pDbl[i]->getReal(), pDbl[i]->getImg(), pDbl[i]->getSize());
            if (!pData[i])
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), "balanc");
                return types::Function::Error;
            }
        }
        else
        {
            pData[i] = pDbl[i]->getReal();
        }

        pDblRes[i] = new types::Double(pDbl[i]->getRows(), pDbl[i]->getCols());
    }

    int iRet = iBalancM(pData[0], pData[1], pDbl[0]->getCols(), bComplex, pDblRes[0]->get(), ( pDblRes[1] ? pDblRes[1]->get() : NULL));

    if (iRet != 0)
    {
        Scierror(999, _("%s: LAPACK error n°%d.\n"), "balanc", iRet);
        return types::Function::Error;
    }

    if (bComplex)
    {
        for (int i = 0; i < in.size(); i++)
        {
            vGetPointerFromDoubleComplex((doublecomplex*)pData[i], pDbl[i]->getSize(), pDbl[i]->getReal(), pDbl[i]->getImg());
            vFreeDoubleComplexFromPointer((doublecomplex*)pData[i]);
        }
    }

    if (_iRetCount == 2)
    {
        out.push_back(pDbl[0]);
        out.push_back(pDblRes[0]);
    }
    else //_iRetCount == 4
    {
        out.push_back(pDbl[0]);
        out.push_back(pDbl[1]);
        out.push_back(pDblRes[0]);
        out.push_back(pDblRes[1]);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

