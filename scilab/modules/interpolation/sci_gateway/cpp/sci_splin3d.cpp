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
#include "tlist.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
#include "interpolation.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_splin3d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //input
    types::Double* pDblXYZ[3]   = {NULL, NULL, NULL};
    types::Double* pDblV        = NULL;

    int pdOrder[3] = {4, 4, 4};

    // output
    types::TList* pTListOut = NULL;

    const wchar_t * wcsStr[] = {L"tensbs3d", L"tx", L"ty", L"tz", L"order", L"bcoef", L"xyzminmax"};

    // *** check the minimal number of input args. ***
    if (in.size() < 4 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "splin3d", 4, 5);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "splin3d", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // x y z
    for (int i = 0; i < 3; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "splin3d", i + 1);
            return types::Function::Error;
        }

        pDblXYZ[i] = in[i]->getAs<types::Double>();

        if (pDblXYZ[i]->isComplex())
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin3d", i + 1);
            return types::Function::Error;
        }

        if (pDblXYZ[i]->getRows() != 1 || pDblXYZ[i]->getSize() < 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A row vector of size at least 3 expected.\n"), "splin3d", i + 1);
            return types::Function::Error;
        }

        if (good_order(pDblXYZ[i]->get(), pDblXYZ[i]->getSize()) == false) /* verify strict increasing abscissae */
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), "splin3d", i + 1);
            return types::Function::Error;
        }
    }

    // v
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "splin3d", 4);
        return types::Function::Error;
    }
    pDblV = in[3]->getAs<types::Double>();

    if (pDblV->getDims() != 3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A real three dimension hypermatrix expected.\n"), "splin3d", 4);
        return types::Function::Error;
    }

    for (int i = 0; i < 3; i++)
    {
        if (pDblV->getDimsArray()[i] != pDblXYZ[i]->getSize())
        {
            Scierror(999, _("%s: Wrong size for dimension %d for input argument #%d : A size of %d expected.\n"), "splin3d", i + 1, 4, pDblXYZ[i]->getSize());
            return types::Function::Error;
        }
    }

    if (in.size() == 5)
    {
        if (in[4]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "splin3d", 5);
            return types::Function::Error;
        }

        types::Double* pDblOrder = in[4]->getAs<types::Double>();

        if (pDblOrder->getSize() != 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A real vector of size 3 expected.\n"), "splin3d", 5);
            return types::Function::Error;
        }

        pdOrder[0] = static_cast<int>(pDblOrder->get(0));
        pdOrder[1] = static_cast<int>(pDblOrder->get(1));
        pdOrder[2] = static_cast<int>(pDblOrder->get(2));

        for (int i = 0; i < 3; i++)
        {
            if (pdOrder[i] < 2 || pdOrder[i] >= pDblXYZ[i]->getSize())
            {
                Scierror(999, _("%s: Wrong value for element %d of input argument #%d : At least 2 and at most %d expected.\n"), "splin3d", i + 1, 5, pDblXYZ[i]->getSize());
                return types::Function::Error;
            }
        }
    }

    // *** Perform operation. ***
    types::String* pStr = new types::String(1, 7);
    pStr->set(wcsStr);

    types::Double* pDblOutOrder = new types::Double(3, 1);
    pDblOutOrder->set(0, static_cast<double>(pdOrder[0]));
    pDblOutOrder->set(1, static_cast<double>(pdOrder[1]));
    pDblOutOrder->set(2, static_cast<double>(pdOrder[2]));

    types::Double* pDblXYZMinMax = new types::Double(6, 1);

    int size[3];
    int xyzSize[3];
    int nxyz = 1;
    int max  = 0;

    for (int i = 0; i < 3; i++)
    {
        xyzSize[i] = pDblXYZ[i]->getSize();
        size[i] = pdOrder[i] + xyzSize[i];
        int temp = pdOrder[i] * (xyzSize[i] + 1);
        nxyz *= xyzSize[i];
        if (max < temp)
        {
            max = temp;
        }

        pDblXYZMinMax->set(i * 2, pDblXYZ[i]->get(0));
        pDblXYZMinMax->set(i * 2 + 1, pDblXYZ[i]->get(xyzSize[i] - 1));
    }

    double* work = new double[nxyz + 2 * max];

    types::Double* pDblOutX = new types::Double(size[0], 1);
    types::Double* pDblOutY = new types::Double(size[1], 1);
    types::Double* pDblOutZ = new types::Double(size[2], 1);
    types::Double* pDblCoef = new types::Double(nxyz, 1);

    int flag = 0;

    C2F(db3ink)(pDblXYZ[0]->get(), &xyzSize[0], pDblXYZ[1]->get(), &xyzSize[1], pDblXYZ[2]->get(), &xyzSize[2],
                pDblV->get(), &xyzSize[0], &xyzSize[1], &pdOrder[0], &pdOrder[1], &pdOrder[2],
                pDblOutX->get(), pDblOutY->get(), pDblOutZ->get(), pDblCoef->get(), work, &flag);

    //  find in dtensbs.f :
    //   IFLAG   Integer scalar.
    //           On input:  0 == knot sequence chosen by B2INK
    //                      1 == knot sequence chosen by user.
    //           On output: 1 == successful execution
    //                      2 == IFLAG out of range
    //                      3 == NX out of range                // NX = xyzSize[0]
    //                      4 == KX out of range                // KX = pdOrder[0]
    //                      5 == X not strictly increasing
    //                      6 == TX not non-decreasing          //these are chosen by DB3INK.(flag = 0)
    //                      7 == NY out of range
    //                      8 == KY out of range
    //                      9 == Y not strictly increasing
    //                     10 == TY not non-decreasing
    //                     11 == NZ out of range
    //                     12 == KZ out of range
    //                     13 == Z not strictly increasing
    //                     14 == TY not non-decreasing          // probably TZ

    delete[] work;

    if (flag != 1) // flag can never be deffirent to 1.
    {
        Scierror(999, _("%s: Problem with 'flag' = %d\n"), "splin3d", flag);
        return types::Function::Error;
    }

    // *** Return result in Scilab. ***
    pTListOut = new types::TList();

    pTListOut->append(pStr);
    pTListOut->append(pDblOutX);
    pTListOut->append(pDblOutY);
    pTListOut->append(pDblOutZ);
    pTListOut->append(pDblOutOrder);
    pTListOut->append(pDblCoef);
    pTListOut->append(pDblXYZMinMax);

    out.push_back(pTListOut);

    return types::Function::OK;
}

