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

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_interp2d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXp = NULL;
    types::Double* pDblYp = NULL;
    types::Double* pDblX  = NULL;
    types::Double* pDblY  = NULL;
    types::Double* pDblC  = NULL;

    // output
    types::Double* pDblZp       = NULL;
    types::Double* pDblDzpdx    = NULL;
    types::Double* pDblDzpdy    = NULL;
    types::Double* pDblD2zdx2p  = NULL;
    types::Double* pDblD2zdxyp  = NULL;
    types::Double* pDblD2zdy2p  = NULL;

    int iType       = 8; // default C0
    int sizeOfX     = 0;
    int sizeOfXp    = 0;
    int sizeOfY     = 0;
    int sizeOfC     = 0;

    // *** check the minimal number of input args. ***
    if (in.size() < 5 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "interp2d", 5, 6);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 6)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "interp2d", 1, 6);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp2d", 1);
        return types::Function::Error;
    }

    pDblXp = in[0]->getAs<types::Double>();
    sizeOfXp = pDblXp->getSize();

    if (pDblXp->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp2d", 1);
        return types::Function::Error;
    }

    // yp
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp2d", 2);
        return types::Function::Error;
    }
    pDblYp = in[1]->getAs<types::Double>();

    if (pDblXp->getRows() != pDblYp->getRows() || pDblXp->getCols() != pDblYp->getCols())
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "interp2d", 1, 2);
        return types::Function::Error;
    }

    if (pDblYp->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp2d", 2);
        return types::Function::Error;
    }

    // x
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp2d", 3);
        return types::Function::Error;
    }

    pDblX = in[2]->getAs<types::Double>();
    sizeOfX = pDblX->getSize();

    if (pDblX->getRows() != 1 || pDblX->getSize() < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d: A row vector of size at least 2 expected.\n"), "interp2d", 3);
        return types::Function::Error;
    }

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp2d", 3);
        return types::Function::Error;
    }

    // y
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp2d", 4);
        return types::Function::Error;
    }

    pDblY = in[3]->getAs<types::Double>();
    sizeOfY = pDblY->getSize();

    if (pDblY->getRows() != 1 || pDblY->getSize() < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d: A row vector of size at least 2 expected.\n"), "interp2d", 4);
        return types::Function::Error;
    }

    // c
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp2d", 5);
        return types::Function::Error;
    }

    pDblC = in[4]->getAs<types::Double>();
    sizeOfC = 16 * (sizeOfX - 1) * (sizeOfY - 1);
    if (pDblC->getCols() != 1 || pDblC->getSize() != sizeOfC)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d: A column vector of size %d expected.\n"), "interp2d", 5, sizeOfC);
        return types::Function::Error;
    }

    // out mode
    if (in.size() == 6)
    {
        if (in[5]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "interp2d", 6);
            return types::Function::Error;
        }

        wchar_t* wcsType = in[5]->getAs<types::String>()->get(0);

        if (wcscmp(wcsType, L"C0") == 0)
        {
            iType = 8;
        }
        else if (wcscmp(wcsType, L"by_zero") == 0)
        {
            iType = 7;
        }
        else if (wcscmp(wcsType, L"natural") == 0)
        {
            iType = 1;
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
            Scierror(999, _("%s: Wrong values for input argument #%d : '%s' is an unknown '%s' type.\n"), "interp2d", 6, wcsType, "outmode");
            return types::Function::Error;
        }
    }

    // *** Perform operation. ***
    pDblZp = new types::Double(pDblXp->getRows(), pDblXp->getCols());

    if (_iRetCount == 1)
    {
        C2F(bicubicinterp)(pDblX->get(), pDblY->get(), pDblC->get(), &sizeOfX, &sizeOfY, pDblXp->get(), pDblYp->get(), pDblZp->get(), &sizeOfXp, &iType);

    }
    else // if(_iRetCount > 2)
    {
        pDblDzpdx = new types::Double(pDblXp->getRows(), pDblXp->getCols());
        pDblDzpdy = new types::Double(pDblXp->getRows(), pDblXp->getCols());

        if (_iRetCount == 3)
        {
            C2F(bicubicinterpwithgrad)(pDblX->get(), pDblY->get(), pDblC->get(), &sizeOfX, &sizeOfY,
                                       pDblXp->get(), pDblYp->get(), pDblZp->get(), pDblDzpdx->get(),
                                       pDblDzpdy->get(), &sizeOfXp, &iType);
        }
        else // == 6
        {
            pDblD2zdx2p = new types::Double(pDblXp->getRows(), pDblXp->getCols());
            pDblD2zdxyp = new types::Double(pDblXp->getRows(), pDblXp->getCols());
            pDblD2zdy2p = new types::Double(pDblXp->getRows(), pDblXp->getCols());

            C2F(bicubicinterpwithgradandhes)(pDblX->get(), pDblY->get(), pDblC->get(), &sizeOfX, &sizeOfY,
                                             pDblXp->get(), pDblYp->get(), pDblZp->get(), pDblDzpdx->get(),
                                             pDblDzpdy->get(),  pDblD2zdx2p->get(), pDblD2zdxyp->get(),
                                             pDblD2zdy2p->get(), &sizeOfXp, &iType);
        }
    }

    // *** Return result in Scilab. ***
    switch (_iRetCount)
    {
        case 6 :
            out.insert(out.begin(), pDblD2zdy2p);
        case 5 :
            out.insert(out.begin(), pDblD2zdxyp);
        case 4 :
            out.insert(out.begin(), pDblD2zdx2p);
        case 3 :
            out.insert(out.begin(), pDblDzpdy);
        case 2 :
            out.insert(out.begin(), pDblDzpdx);
        default :
            break;
    }

    out.insert(out.begin(), pDblZp);

    return types::Function::OK;
}

