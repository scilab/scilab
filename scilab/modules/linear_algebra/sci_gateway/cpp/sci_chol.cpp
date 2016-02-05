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
#include "chol.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_chol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl     = NULL;
    double* pdOut           = NULL;
    int iCholProductResult  = 0;
    types::Double* result   = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "chol", 1);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_chol";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>();
    if (pDbl->getRows() != pDbl->getCols())
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "chol", 1);
        return types::Function::Error;
    }

    if (pDbl->getRows() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else if (pDbl->getRows() == -1) // manage eye case
    {
        if (pDbl->get(0) <= 0)
        {
            /* Matrix must be positive definite */
            Scierror(29, _("%s: Matrix is not positive definite.\n"), "chol");
            return types::Function::Error;
        }

        types::Double* result = new types::Double(sqrt(pDbl->get(0)));
        out.push_back(result);
        return types::Function::OK;
    }

    if (pDbl->isComplex())
    {
        doublecomplex* poData = oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        iCholProductResult = iComplexCholProduct(poData, pDbl->getRows());

        result = new types::Double(pDbl->getRows(), pDbl->getCols(), true);

        vGetPointerFromDoubleComplex(poData, pDbl->getSize(), result->getReal(), result->getImg());
        vFreeDoubleComplexFromPointer(poData);
    }
    else
    {
        result = pDbl->clone()->getAs<types::Double>();
        iCholProductResult = iRealCholProduct(result->get(), pDbl->getRows());
    }

    if (iCholProductResult > 0)
    {
        /* Matrix must be positive definite */
        Scierror(29, _("%s: Matrix is not positive definite.\n"), "chol");
        return types::Function::Error;
    }

    out.push_back(result);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

