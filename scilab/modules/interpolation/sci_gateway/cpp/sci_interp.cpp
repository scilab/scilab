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

types::Function::ReturnValue sci_interp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXp       = NULL;
    types::Double* pDblX        = NULL;
    types::Double* pDblY        = NULL;
    types::Double* pDblD        = NULL;

    // output
    types::Double* pDblYp  = NULL;
    types::Double* pDblYp1 = NULL;
    types::Double* pDblYp2 = NULL;
    types::Double* pDblYp3 = NULL;

    int iType       = 8; // default C0
    int sizeOfXp    = 0;
    int sizeOfX     = 0;

    // *** check the minimal number of input args. ***
    if (in.size() < 4 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "interp", 4, 5);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "interp", 1, 4);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp", 1);
        return types::Function::Error;
    }

    pDblXp = in[0]->getAs<types::Double>();
    sizeOfXp = pDblXp->getSize();

    if (pDblXp->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp", 1);
        return types::Function::Error;
    }

    // x
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp", 2);
        return types::Function::Error;
    }

    pDblX = in[1]->getAs<types::Double>();
    if ((pDblX->getCols() != 1 && pDblX->getRows() != 1) || pDblX->getSize() < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d: A vector of size at least 2 expected.\n"), "interp", 2);
        return types::Function::Error;
    }
    sizeOfX = pDblX->getSize();

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp", 2);
        return types::Function::Error;
    }

    // y
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp", 3);
        return types::Function::Error;
    }
    pDblY = in[2]->getAs<types::Double>();

    if (pDblY->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp", 3);
        return types::Function::Error;
    }

    // d
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "interp", 4);
        return types::Function::Error;
    }
    pDblD = in[3]->getAs<types::Double>();

    if (pDblD->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "interp", 4);
        return types::Function::Error;
    }

    if ( pDblX->getRows() != pDblY->getRows() ||
            pDblX->getCols() != pDblY->getCols() ||
            pDblX->getRows() != pDblD->getRows() ||
            pDblX->getCols() != pDblD->getCols())
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d to #%d: Same sizes expected.\n"), "interp", 2, 4);
        return types::Function::Error;
    }

    // out mode
    if (in.size() == 5)
    {
        if (in[4]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "interp", 5);
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
        else if (wcscmp(wcsType, L"linear") == 0)
        {
            iType = 9;
        }
        else // undefined
        {
            Scierror(999, _("%s: Wrong values for input argument #%d : '%s' is an unknown '%s' type.\n"), "interp", 5, wcsType, "outmode");
            return types::Function::Error;
        }
    }

    // *** Perform operation. ***
    pDblYp  = new types::Double(pDblXp->getRows(), pDblXp->getCols());
    pDblYp1 = new types::Double(pDblXp->getRows(), pDblXp->getCols());
    pDblYp2 = new types::Double(pDblXp->getRows(), pDblXp->getCols());
    pDblYp3 = new types::Double(pDblXp->getRows(), pDblXp->getCols());

    C2F(evalpwhermite)(pDblXp->get(), pDblYp->get(), pDblYp1->get(), pDblYp2->get(), pDblYp3->get(), &sizeOfXp, pDblX->get(), pDblY->get(), pDblD->get(), &sizeOfX, &iType);

    // *** Return result in Scilab. ***
    out.push_back(pDblYp);
    if (_iRetCount > 1)
    {
        out.push_back(pDblYp1);
    }

    if (_iRetCount > 2)
    {
        out.push_back(pDblYp2);
    }

    if (_iRetCount > 3)
    {
        out.push_back(pDblYp3);
    }

    return types::Function::OK;
}

