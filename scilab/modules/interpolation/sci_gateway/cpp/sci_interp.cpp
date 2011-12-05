/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"interp", 4, 5);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 4)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"interp", 1, 4);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp
    if (in[0]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"interp", 1);
        return types::Function::Error;
    }

    pDblXp = in[0]->getAs<types::Double>();
    sizeOfXp = pDblXp->getSize();
    // x
    if (in[1]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"interp", 2);
        return types::Function::Error;
    }

    pDblX = in[1]->getAs<types::Double>();
    if ((pDblX->getCols() != 1 && pDblX->getRows() != 1) || pDblX->getSize() < 2)
    {
        ScierrorW(999, _W("%ls: Wrong size for input arguments #%d: A vector of size at least 2 expected.\n"), L"interp", 2);
        return types::Function::Error;
    }
    sizeOfX = pDblX->getSize();

    // y
    if (in[2]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"interp", 3);
        return types::Function::Error;
    }
    pDblY = in[2]->getAs<types::Double>();

    // d
    if (in[3]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"interp", 4);
        return types::Function::Error;
    }
    pDblD = in[3]->getAs<types::Double>();

    if ( pDblX->getRows() != pDblY->getRows() ||
            pDblX->getCols() != pDblY->getCols() ||
            pDblX->getRows() != pDblD->getRows() ||
            pDblX->getCols() != pDblD->getCols())
    {
        ScierrorW(999, _W("%ls: Wrong size for input arguments #%d to #%d: Same sizes expected.\n"), L"interp", 2, 4);
        return types::Function::Error;
    }

    // out mode
    if (in.size() == 5)
    {
        if (in[4]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A string expected.\n"), L"interp", 5);
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
            ScierrorW(999, _W("%ls: Wrong values for input argument #%d : '%ls' is a unknow '%ls' type.\n"), L"interp", 5, wcsType, L"outmode");
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

