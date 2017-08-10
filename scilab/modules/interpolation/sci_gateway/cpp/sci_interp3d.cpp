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
#include "interpolation_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "tlist.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_interp3d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXYZ[3]       = {NULL, NULL, NULL};
    types::TList* pTList            = NULL;
    types::Double* pDblX            = NULL;
    types::Double* pDblY            = NULL;
    types::Double* pDblZ            = NULL;
    types::Double* pDblOrder        = NULL;
    types::Double* pDblCoef         = NULL;
    types::Double* pDblXyzminmax    = NULL;

    // output
    types::Double* pDblFp   = NULL;
    types::Double* pDblFpdx = NULL;
    types::Double* pDblFpdy = NULL;
    types::Double* pDblFpdz = NULL;

    int iType = 0;
    int order[3];
    int sizeOfXp;

    // *** check the minimal number of input args. ***
    if ((in.size() < 4) || (5 < in.size()))
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "interp3d", 4);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "interp3d", 1, 4);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp yp zp
    for (int i = 0; i < 3; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp3d", i + 1);
            return types::Function::Error;
        }

        pDblXYZ[i] = in[i]->getAs<types::Double>();

        if (pDblXYZ[0]->getRows() != pDblXYZ[i]->getRows() || pDblXYZ[0]->getCols() != pDblXYZ[i]->getCols())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : Same size as argument %d expected.\n"), "interp3d", i + 1, 1);
            return types::Function::Error;
        }

        if (pDblXYZ[i]->isComplex())
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp3d", i + 1);
            return types::Function::Error;
        }
    }

    sizeOfXp = pDblXYZ[0]->getSize();

    if (in[3]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist of type %s expected.\n"), "interp3d", 4, "tensbs3d");
    }

    pTList = in[3]->getAs<types::TList>();

    if (pTList->getTypeStr() != L"tensbs3d")
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A %s tlist expected.\n"), "interp3d", 4, "tensbs3d");
        return types::Function::Error;
    }

    pDblX = pTList->getField(L"tx")->getAs<types::Double>();
    pDblY = pTList->getField(L"ty")->getAs<types::Double>();
    pDblZ = pTList->getField(L"tz")->getAs<types::Double>();
    pDblOrder = pTList->getField(L"order")->getAs<types::Double>();
    pDblCoef = pTList->getField(L"bcoef")->getAs<types::Double>();
    pDblXyzminmax = pTList->getField(L"xyzminmax")->getAs<types::Double>();

    if (in.size() == 5)
    {
        if (in[4]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "interp3d", 5);
            return types::Function::Error;
        }

        wchar_t* wcsType = in[4]->getAs<types::String>()->get(0);

        if (wcscmp(wcsType, L"C0") == 0)
        {
            iType = 8;
        }
        else if (wcscmp(wcsType, L"by_zero") == 0)
        {
            iType = 7;
        }
        else if (wcscmp(wcsType, L"periodic") == 0)
        {
            iType = 3;
        }
        else if (wcscmp(wcsType, L"by_nan") == 0)
        {
            iType = 10;
        }
        else // undefined
        {
            char* pstType = wide_string_to_UTF8(wcsType);
            Scierror(999, _("%s: Wrong values for input argument #%d : '%s' is an unknown '%s' type.\n"), "interp3d", 5, pstType, "outmode");
            FREE(pstType);
            return types::Function::Error;
        }
    }
    else
    {
        //"C0"
        iType = 8;
    }

    // *** Perform operation. ***
    pDblFp = new types::Double(pDblXYZ[0]->getRows(), pDblXYZ[0]->getCols());

    order[0] = static_cast<int>(pDblOrder->get(0));
    order[1] = static_cast<int>(pDblOrder->get(1));
    order[2] = static_cast<int>(pDblOrder->get(2));

    int sizeOfX = pDblX->getRows() - order[0];
    int sizeOfY = pDblY->getRows() - order[1];
    int sizeOfZ = pDblZ->getRows() - order[2];

    double* minmax = pDblXyzminmax->get();

    int workSize = order[1] * order[2] + 3 * std::max(order[0], std::max(order[1], order[2])) + order[2];
    double* work = new double[workSize];

    if (_iRetCount == 1)
    {
        C2F(driverdb3val)(pDblXYZ[0]->get(), pDblXYZ[1]->get(), pDblXYZ[2]->get(), pDblFp->get(), &sizeOfXp,
                          pDblX->get(), pDblY->get(), pDblZ->get(), &sizeOfX, &sizeOfY, &sizeOfZ,
                          &order[0], &order[1], &order[2], pDblCoef->get(), work,
                          &minmax[0], &minmax[1], &minmax[2], &minmax[3], &minmax[4], &minmax[5], &iType);
    }
    else // _iRetCount == 4
    {
        pDblFpdx = new types::Double(pDblXYZ[0]->getRows(), pDblXYZ[0]->getCols());
        pDblFpdy = new types::Double(pDblXYZ[0]->getRows(), pDblXYZ[0]->getCols());
        pDblFpdz = new types::Double(pDblXYZ[0]->getRows(), pDblXYZ[0]->getCols());

        C2F(driverdb3valwithgrad)(pDblXYZ[0]->get(), pDblXYZ[1]->get(), pDblXYZ[2]->get(),
                                  pDblFp->get(), pDblFpdx->get(), pDblFpdy->get(), pDblFpdz->get(),
                                  &sizeOfXp, pDblX->get(), pDblY->get(), pDblZ->get(),
                                  &sizeOfX, &sizeOfY, &sizeOfZ, &order[0], &order[1], &order[2], pDblCoef->get(), work,
                                  &minmax[0], &minmax[1], &minmax[2], &minmax[3], &minmax[4], &minmax[5], &iType);
    }

    delete[] work;

    // *** Return result in Scilab. ***
    switch (_iRetCount)
    {
        case 4 :
            out.insert(out.begin(), pDblFpdz);
        case 3 :
            out.insert(out.begin(), pDblFpdy);
        case 2 :
            out.insert(out.begin(), pDblFpdx);
        default :
            break;
    }

    out.insert(out.begin(), pDblFp);

    return types::Function::OK;
}

