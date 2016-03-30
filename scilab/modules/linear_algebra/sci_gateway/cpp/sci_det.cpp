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
#include "det.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_det(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl             = NULL;
    types::Double* pDblMantissa     = NULL;
    types::Double* pDblExponent     = NULL;
    double* pData                   = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "det", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "det", 1, 2);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_det";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

    if (pDbl->isComplex())
    {
        pData = (double *)oGetDoubleComplexFromPointer(pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
        if (!pData)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), "det");
            return types::Function::Error;
        }
    }
    else
    {
        pData = pDbl->getReal();
    }

    if (pDbl->getRows() != pDbl->getCols())
    {
        Scierror(20, _("%s: Wrong type for input argument #%d: Square matrix expected.\n"), "det", 1);
        return types::Function::Error;
    }

    if ((pDbl->getRows() == -1)) // manage eye case
    {
        Scierror(271, _("%s: Size varying argument a*eye(), (arg %d) not allowed here.\n"), "det", 1);
        return types::Function::Error;
    }

    pDblMantissa = new types::Double(1, 1, pDbl->isComplex());

    if (_iRetCount == 2)
    {
        pDblExponent = new types::Double(1, 1);
    }

    int iExponent = 0;
    int iRet = iDetM(pData, pDbl->getCols(), pDblMantissa->getReal(), pDbl->isComplex() ? pDblMantissa->getImg() : NULL, pDblExponent ? &iExponent : NULL);
    if (iRet < 0)
    {
        Scierror(999, _("%s: LAPACK error n°%d.\n"), "det", iRet);
        return types::Function::Error;
    }

    if (pDblExponent)
    {
        pDblExponent->set(0, iExponent);
    }

    if (pDbl->isComplex())
    {
        vFreeDoubleComplexFromPointer((doublecomplex*)pData);
    }

    if (_iRetCount == 2)
    {
        out.push_back(pDblExponent);
    }

    delete pDbl;
    out.push_back(pDblMantissa);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

