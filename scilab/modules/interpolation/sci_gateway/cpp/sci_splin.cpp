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
#include "interpolation.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_splin(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblX    = NULL;
    types::Double* pDblY    = NULL;
    types::Double* pDblDer  = NULL;

    // output
    types::Double* pDblOut  = NULL;

    int iType   = 0; // default value = not_a_knot
    int one     = 1;
    int iSize   = 0;

    double* rwork1 = NULL;
    double* rwork2 = NULL;
    double* rwork3 = NULL;
    double* rwork4 = NULL;

    // *** check the minimal number of input args. ***
    if (in.size() < 2 || in.size() > 4)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"splin", 2, 4);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"splin", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // x
    if (in[0]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"splin", 1);
        return types::Function::Error;
    }

    pDblX = in[0]->getAs<types::Double>();
    iSize = pDblX->getSize();

    if (iSize < 2)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d : At least a size of 2 expected.\n"), L"splin", 1);
        return types::Function::Error;
    }

    if (good_order(pDblX->get(), iSize) == false) /* verify strict increasing abscissae */
    {
        ScierrorW(999, _W("%ls: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), L"splin", 1);
        return types::Function::Error;
    }

    // y
    if (in[1]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"splin", 2);
        return types::Function::Error;
    }

    pDblY = in[1]->getAs<types::Double>();

    if ( pDblX->getCols() != pDblY->getCols() ||
            pDblX->getRows() != pDblY->getRows() ||
            (pDblX->getCols() != 1 && pDblX->getRows() != 1))
    {
        ScierrorW(999, _W("%ls: Wrong size for input arguments #%d and #%d: Vector of same size expected.\n"), L"splin", 1, 2);
        return types::Function::Error;
    }

    if (in.size() > 2)
    {
        if (in[2]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A string expected.\n"), L"splin", 3);
            return types::Function::Error;
        }

        wchar_t* wcsType = in[2]->getAs<types::String>()->get(0);

        if (wcscmp(wcsType, L"not_a_knot") == 0)
        {
            iType = 0;
        }
        else if (wcscmp(wcsType, L"natural") == 0)
        {
            iType = 1;
        }
        else if (wcscmp(wcsType, L"clamped") == 0)
        {
            iType = 2;
        }
        else if (wcscmp(wcsType, L"periodic") == 0)
        {
            iType = 3;
        }
        else if (wcscmp(wcsType, L"fast") == 0)
        {
            iType = 4;
        }
        else if (wcscmp(wcsType, L"fast_periodic") == 0)
        {
            iType = 5;
        }
        else if (wcscmp(wcsType, L"monotone") == 0)
        {
            iType = 6;
        }
        else // undefined
        {
            ScierrorW(999, _W("%ls: Wrong values for input argument #%d : '%ls' is a unknow '%ls' type.\n"), L"splin", 3, wcsType, L"spline");
            return types::Function::Error;
        }

        if (iType == 2)
        {
            if (in.size() != 4)
            {
                ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"splin", 4);
                return types::Function::Error;
            }

            if (in[3]->isDouble() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"splin", 4);
                return types::Function::Error;
            }

            pDblDer = in[3]->getAs<types::Double>();

            if (pDblDer->getSize() != 2)
            {
                ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A martix of size 2 expected.\n"), L"splin", 4);
                return types::Function::Error;
            }
        }
        else if (in.size() == 4)
        {
            ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"splin", 3);
            return types::Function::Error;
        }
    }

    // verify y(1) = y(n) for periodic splines
    if ((iType == 3 || iType == 5) && pDblY->get(0) != pDblY->get(pDblY->getSize() - 1))
    {
        ScierrorW(999, _W("%ls: Wrong value for periodic spline %s: Must be equal to %s.\n"), L"spline", L"y(1)", L"y(n)");
        return types::Function::Error;
    }

    // *** Perform operation. ***
    pDblOut = new types::Double(pDblX->getRows(), pDblX->getCols());

    switch (iType)
    {
        case 6:
            C2F(dpchim)(&iSize, pDblX->get(), pDblY->get(), pDblOut->get(), &one);
        case 5:
        case 4:
            C2F(derivd)(pDblX->get(), pDblY->get(), pDblOut->get(), &iSize, &one, &iType);
            break;
            break;
        case 3:
        case 2:
        case 1:
        case 0:
        {
            rwork1 = new double[iSize];
            rwork2 = new double[iSize - 1];
            rwork3 = new double[iSize - 1];
            rwork4 = rwork1;

            if (iType == 2)
            {
                pDblOut->set(0, pDblDer->get(0));
                pDblOut->set(iSize - 1, pDblDer->get(1));
            }

            if (iType == 3)
            {
                rwork4 = new double[iSize - 1];
            }

            C2F(splinecub)(pDblX->get(), pDblY->get(), pDblOut->get(), &iSize, &iType, rwork1, rwork2, rwork3, rwork4);

            delete[] rwork1;
            delete[] rwork2;
            delete[] rwork3;
            if (iType == 3)
            {
                delete[] rwork4;
            }

            break;
        }
    }

    // *** Return result in Scilab. ***
    out.push_back(pDblOut);

    return types::Function::OK;
}

