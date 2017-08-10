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
#include "sciprint.h"
#include "invert_matrix.h"
#include "configvariable_interface.h"
#include "doublecomplex.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_inv(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDbl = NULL;
    double* pData       = NULL;
    int ret             = 0;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "inv", 1);
        return types::Function::Error;
    }

    if ((in[0]->isDouble() == false))
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_inv";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDbl = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>(); // input data will be modified

    if (pDbl->getRows() != pDbl->getCols())
    {
        Scierror(20, _("%s: Wrong type for argument %d: Square matrix expected.\n"), "inv", 1);
        return types::Function::Error;
    }

    if (pDbl->getRows() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (pDbl->isComplex())
    {
        /* c -> z */
        pData = (double*)oGetDoubleComplexFromPointer( pDbl->getReal(), pDbl->getImg(), pDbl->getSize());
    }
    else
    {
        pData = pDbl->getReal();
    }

    if (pDbl->getCols() == -1)
    {
        pData[0] = 1. / pData[0];
    }
    else
    {
        double dblRcond;
        ret = iInvertMatrixM(pDbl->getRows(), pDbl->getCols(), pData, pDbl->isComplex(), &dblRcond);
        if (pDbl->isComplex())
        {
            /* z -> c */
            vGetPointerFromDoubleComplex((doublecomplex*)pData, pDbl->getSize(), pDbl->getReal(), pDbl->getImg());
            vFreeDoubleComplexFromPointer((doublecomplex*)pData);
        }

        if (ret == -1)
        {
            if (getWarningMode())
            {
                sciprint(_("Warning :\n"));
                sciprint(_("matrix is close to singular or badly scaled. rcond = %1.4E\n"), dblRcond);
            }
        }
    }

    if (ret == 19)
    {
        Scierror(19, _("%s: Problem is singular.\n"), "inv");
        return types::Function::Error;
    }

    out.push_back(pDbl);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

