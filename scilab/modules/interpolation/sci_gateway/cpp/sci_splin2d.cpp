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
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "interpolation_functions.h"
#include "interpolation.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_splin2d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblX    = NULL;
    types::Double* pDblY    = NULL;
    types::Double* pDblZ    = NULL;

    // output
    types::Double* pDblOut  = NULL;

    int iType   = 0; // default value = not_a_knot
    int sizeOfX = 0;
    int sizeOfY = 0;

    // *** check the minimal number of input args. ***
    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "splin2d", 3, 4);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "splin2d", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // x
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A vector expected.\n"), "splin2d", 1);
        return types::Function::Error;
    }

    pDblX = in[0]->getAs<types::Double>();
    sizeOfX = pDblX->getSize();

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin2d", 1);
        return types::Function::Error;
    }

    if (pDblX->getRows() != 1 || pDblX->getSize() < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A row vector of size at least 2 expected.\n"), "splin2d", 1);
        return types::Function::Error;
    }

    if (good_order(pDblX->get(), pDblX->getSize()) == false) /* verify strict increasing abscissae */
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), "splin2d", 1);
        return types::Function::Error;
    }

    // y
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A vector expected.\n"), "splin2d", 2);
        return types::Function::Error;
    }

    pDblY = in[1]->getAs<types::Double>();
    sizeOfY = pDblY->getSize();

    if (pDblY->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin2d", 2);
        return types::Function::Error;
    }

    if (pDblY->getRows() != 1 || pDblY->getSize() < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A row vector of size at least 2 expected.\n"), "splin2d", 2);
        return types::Function::Error;
    }

    if (good_order(pDblY->get(), pDblY->getSize()) == false) /* verify strict increasing abscissae */
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), "splin2d", 2);
        return types::Function::Error;
    }

    // z
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "splin2d", 3);
        return types::Function::Error;
    }

    pDblZ = in[2]->getAs<types::Double>();

    if (pDblZ->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "splin2d", 3);
        return types::Function::Error;
    }

    if (pDblZ->getRows() != pDblX->getCols() || pDblZ->getCols() != pDblY->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A matrix of size %d * %d expected.\n"), "splin2d", 3, pDblX->getCols(), pDblY->getCols());
        return types::Function::Error;
    }

    // type
    if (in.size() == 4)
    {
        if (in[3]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "splin2d", 4);
            return types::Function::Error;
        }

        wchar_t* wcsType = in[3]->getAs<types::String>()->get(0);

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
            char* pstType = wide_string_to_UTF8(wcsType);
            Scierror(999, _("%s: Wrong values for input argument #%d : '%s' is an unknown '%s' type.\n"), "splin2d", 4, pstType, "spline");
            FREE(pstType);
            return types::Function::Error;
        }
    }

    // *** Perform operation. ***
    int outSize = 16 * (pDblX->getSize() - 1) * (pDblY->getSize() - 1);
    pDblOut = new types::Double(outSize, 1);

    double* p = new double[pDblZ->getSize()];
    double* q = new double[pDblZ->getSize()];
    double* r = new double[pDblZ->getSize()];

    if (iType > 3)
    {
        C2F(bicubicsubspline)(pDblX->get(), pDblY->get(), pDblZ->get(), &sizeOfX, &sizeOfY, pDblOut->get(), p, q, r, &iType);
    }
    else
    {
        int nxy = std::max(pDblX->getSize(), pDblY->getSize());
        double* lA_d    = new double[nxy    ];
        double* lA_sd   = new double[nxy - 1];
        double* ld      = new double[sizeOfY];
        double* lqdu    = new double[nxy    ];
        double* lutemp  = new double[sizeOfY];
        double* lll = NULL;

        if (iType == 3)
        {
            lll = new double[nxy - 1];
        }

        C2F(bicubicspline)(pDblX->get(), pDblY->get(), pDblZ->get(), &sizeOfX, &sizeOfY, pDblOut->get(),
                           p, q, r, lA_d, lA_sd, ld, lll, lqdu, lutemp, &iType);

        delete[] lA_d;
        delete[] lA_sd;
        delete[] ld;
        delete[] lqdu;
        delete[] lutemp;

        if (iType == 3)
        {
            delete[] lll;
        }
    }

    delete[] p;
    delete[] q;
    delete[] r;

    // *** Return result in Scilab. ***
    out.push_back(pDblOut);

    return types::Function::OK;
}

