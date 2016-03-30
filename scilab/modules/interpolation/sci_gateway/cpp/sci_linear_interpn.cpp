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
#include "someinterp.h"
#include "interpolation.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_linear_interpn(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    std::vector<types::Double*> lpDblXp;
    std::vector<types::Double*> lpDblX;
    types::Double* pDblV = NULL;

    // output
    types::Double* pDblVp  = NULL;

    int n           = 0;
    int iType       = 0;

    // *** check the minimal number of input args. ***
    if (in.size() < 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "linear_interpn", 3);
        return types::Function::Error;
    }

    n = ((int)in.size() + 1) / 2 - 1;

    // *** check number of output args according the methode. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "linear_interpn", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xp1 ... xpn
    for (int i = 0; i < n; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "linear_interpn", i + 1);
            return types::Function::Error;
        }

        lpDblXp.push_back(in[i]->getAs<types::Double>());

        if (i != 0 && (lpDblXp[i - 1]->getRows() != lpDblXp[i]->getRows() || lpDblXp[i - 1]->getCols() != lpDblXp[i]->getCols()))
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"), "linear_interpn", i, i + 1);
            return types::Function::Error;
        }

        if (lpDblXp[i]->isComplex())
        {
            Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "linear_interpn", i + 1);
            return types::Function::Error;
        }
    }

    // x1 ... xn
    for (int i = 0; i < n; i++)
    {
        if (in[i + n]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "linear_interpn", i + 1);
            return types::Function::Error;
        }

        lpDblX.push_back(in[i + n]->getAs<types::Double>());

        if ((lpDblX[i]->getCols() != 1 && lpDblX[i]->getRows() != 1) || lpDblX[i]->getSize() < 2)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d: A vector of size at least 2 expected.\n"), "linear_interpn", i + 1);
            return types::Function::Error;
        }

        /* verify strict increasing abscissae */
        if (good_order(lpDblX[i]->get(), lpDblX[i]->getSize()) == false)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), "linear_interpn", i + 1);
            return types::Function::Error;
        }
    }

    // v
    if (in[2 * n]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "linear_interpn", 2 * n + 1);
        return types::Function::Error;
    }

    pDblV = in[2 * n]->getAs<types::Double>();

    if (lpDblX.size() == 1)
    {
        if (pDblV->getSize() != lpDblX[0]->getSize())
        {
            Scierror(999, _("%s: Size incompatibility between grid points and values in dimension %d.\n"), "linear_interpn", 1);
            return types::Function::Error;
        }
    }
    else if (lpDblX.size() == 2)
    {
        if (pDblV->getRows() != lpDblX[0]->getSize() || pDblV->getCols() != lpDblX[1]->getSize())
        {
            Scierror(999, _("%s: Size incompatibility between grid points and values in dimension %d or %d.\n"), "linear_interpn", 1, 2);
            return types::Function::Error;
        }
    }
    else // hypermatrix
    {
        if (pDblV->getDims() != n)
        {
            Scierror(999, _("%s: %s must be a real %d-dim hypermatrix.\n"), "linear_interpn", "V", n);
            return types::Function::Error;
        }

        for (int i = 0; i < n; i++)
        {
            if (lpDblX[i]->getSize() != pDblV->getDimsArray()[i])
            {
                Scierror(999, _("%s: Size incompatibility between grid points and grid values in dimension %d.\n"), "linear_interpn", i + 1);
                return types::Function::Error;
            }
        }
    }

    // out mode
    if (in.size() == 2 * n + 2)
    {
        if (in[2 * n + 1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "linear_interpn", 2 * n + 2);
            return types::Function::Error;
        }

        wchar_t* wcsType = in[2 * n + 1]->getAs<types::String>()->get(0);

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
            char* pst = wide_string_to_UTF8(wcsType);
            Scierror(999, _("%s: Wrong values for input argument #%d : '%s' is an unknown '%s' type.\n"), "linear_interpn", 2 * n + 3, pst, "outmode");
            FREE(pst);
            return types::Function::Error;
        }
    }

    // *** Perform operation. ***
    pDblVp = new types::Double(lpDblXp[0]->getRows(), lpDblXp[0]->getCols());

    double** x = new double*[n]; // allocate n double*
    double** xp = new double*[n]; // allocate n double*
    int* dim = new int[n];
    int iPow = (int)pow((double)2, n);

    for (int i = 0; i < n; i++)
    {
        x[i]    = lpDblX[i]->get();
        xp[i]   = lpDblXp[i]->get();
        dim[i]  = lpDblX[i]->getSize();
    }

    //working table
    double* u = new double[n];
    double* v = new double[iPow];

    int* ad = new int[iPow];
    int* k  = new int[n];

    nlinear_interp(x, pDblV->get(), dim, n, xp, pDblVp->get(), lpDblXp[0]->getSize(), iType, u, v, ad, k);

    delete[] dim;
    delete[] u;
    delete[] v;
    delete[] ad;
    delete[] k;
    delete[] xp;
    delete[] x;

    // *** Return result in Scilab. ***
    out.push_back(pDblVp);

    return types::Function::OK;
}

