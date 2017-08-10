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
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_bsplin3val(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXYZ[3]       = {NULL, NULL, NULL};
    types::Double* pDblDer          = NULL;
    types::TList*  pTList           = NULL;
    types::Double* pDblX            = NULL;
    types::Double* pDblY            = NULL;
    types::Double* pDblZ            = NULL;
    types::Double* pDblOrder        = NULL;
    types::Double* pDblCoef         = NULL;
    types::Double* pDblXyzminmax    = NULL;

    // output
    types::Double* pDblDfp = NULL;

    // *** check the minimal number of input args. ***
    if (in.size() != 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bsplin3val", 5);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "bsplin3val", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp yp zp
    for (int i = 0; i < 3; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "bsplin3val", i + 1);
            return types::Function::Error;
        }

        pDblXYZ[i] = in[i]->getAs<types::Double>();

        if (pDblXYZ[i]->isComplex())
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "bsplin3val", i + 1);
            return types::Function::Error;
        }

        if (pDblXYZ[0]->getRows() != pDblXYZ[i]->getRows() || pDblXYZ[0]->getCols() != pDblXYZ[i]->getCols())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : Same size as argument %d expected.\n"), "bsplin3val", i + 1, 1);
            return types::Function::Error;
        }
    }


    if (in[3]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist of type bsplin3val expected.\n"), "bsplin3val", 4);
    }

    pTList = in[3]->getAs<types::TList>();

    if (pTList->getTypeStr() != L"tensbs3d")
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A %s tlist expected.\n"), "bsplin3val", 4, "tensbs3d");
        return types::Function::Error;
    }

    pDblX = pTList->getField(L"tx")->getAs<types::Double>();
    pDblY = pTList->getField(L"ty")->getAs<types::Double>();
    pDblZ = pTList->getField(L"tz")->getAs<types::Double>();
    pDblOrder = pTList->getField(L"order")->getAs<types::Double>();
    pDblCoef = pTList->getField(L"bcoef")->getAs<types::Double>();
    pDblXyzminmax = pTList->getField(L"xyzminmax")->getAs<types::Double>();

    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "bsplin3val", 5);
        return types::Function::Error;
    }

    pDblDer = in[4]->getAs<types::Double>();

    if (pDblDer->getSize() != 3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A vector of size 3 expected.\n"), "bsplin3val", 5);
        return types::Function::Error;
    }

    if (    pDblDer->get(0) != floor(pDblDer->get(0)) || pDblDer->get(0) < 0.0
            || pDblDer->get(1) != floor(pDblDer->get(1)) || pDblDer->get(1) < 0.0
            || pDblDer->get(2) != floor(pDblDer->get(2)) || pDblDer->get(2) < 0.0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d : A real positive integer vector expected.\n"), "bsplin3val", 5);
        return types::Function::Error;
    }

    // *** Perform operation. ***
    pDblDfp = new types::Double(pDblXYZ[0]->getRows(), pDblXYZ[0]->getCols());

    int der[3];
    der[0] = static_cast<int>(pDblDer->get(0));
    der[1] = static_cast<int>(pDblDer->get(1));
    der[2] = static_cast<int>(pDblDer->get(2));

    int order[3];
    order[0] = static_cast<int>(pDblOrder->get(0));
    order[1] = static_cast<int>(pDblOrder->get(1));
    order[2] = static_cast<int>(pDblOrder->get(2));

    int sizeOfX = pDblX->getRows() - order[0];
    int sizeOfY = pDblY->getRows() - order[1];
    int sizeOfZ = pDblZ->getRows() - order[2];

    double* minmax = pDblXyzminmax->get();

    int workSize = order[1] * order[2] + 3 * std::max(order[0], std::max(order[1], order[2])) + order[2];
    double* work = new double[workSize];

    for (int i = 0; i < pDblXYZ[0]->getSize(); i++)
    {
        double ret = C2F(db3val)(pDblXYZ[0]->get() + i, pDblXYZ[1]->get() + i, pDblXYZ[2]->get() + i,
                                 &der[0], &der[1], &der[2], pDblX->get(), pDblY->get(), pDblZ->get(),
                                 &sizeOfX, &sizeOfY, &sizeOfZ, &order[0], &order[1], &order[2], pDblCoef->get(), work);
        pDblDfp->set(i, ret);
    }

    delete[] work;

    // *** Return result in Scilab. ***
    out.push_back(pDblDfp);

    return types::Function::OK;
}

