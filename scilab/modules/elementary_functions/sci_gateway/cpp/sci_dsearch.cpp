/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dsearch(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblIn       = NULL;
    types::Double* pDblSearch   = NULL;

    // output
    types::Double* pDblInd      = NULL;
    types::Double* pDblOcc      = NULL;

    double* pdOcc               = NULL;
    double dInfo                = 0;

    int iTypeSearch = 1; // 1 = 'c' || 0 = 'd'

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dsearch", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): At most %d expected.\n"), "dsearch", 3);
        return types::Function::Error;
    }

    /***** get data *****/
    // input data
    if (in[0]->isDouble() == false || in[1]->isDouble() == false)
    {
        std::wstring stType;
        //build manually overload name
        if (in[0]->isDouble() == false)
        {
            stType = in[0]->getShortTypeStr();
        }
        else
        {
            stType = in[1]->getShortTypeStr();
        }

        return Overload::call(L"%" + stType + L"_dsearch", in, _iRetCount, out);
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "dsearch", 1);
        return types::Function::Error;
    }

    // what to look for in input data
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), "dsearch", 2);
        return types::Function::Error;
    }

    pDblSearch = in[1]->getAs<types::Double>();

    if ((pDblSearch->getCols() != 1 && pDblSearch->getRows() != 1) || pDblSearch->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "dsearch", 2);
        return types::Function::Error;
    }

    for (int i = 1; i < pDblSearch->getSize(); i++)
    {
        if (pDblSearch->get(i - 1) >= pDblSearch->get(i))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A strictly increasing order expected.\n"), "dsearch", 2);
            return types::Function::Error;
        }
    }

    // search type
    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "dsearch", 3);
            return types::Function::Error;
        }

        wchar_t* wcsString = in[2]->getAs<types::String>()->get(0);

        if (wcscmp(wcsString, L"c") == 0)
        {
            iTypeSearch = 1;
        }
        else if (wcscmp(wcsString, L"d") == 0)
        {
            iTypeSearch = 0;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: It must be 'c' or 'd'.\n"), "dsearch", 3);
            return types::Function::Error;
        }
    }

    /***** perform operation *****/
    int iRowsOcc = pDblSearch->getRows();
    int iColsOcc = pDblSearch->getCols();

    // If iTypeSearch= 1 ("c") , a (n-1)-by-1 or 1-by-(n-1) of doubles. If iTypeSearch=0 ("d") (0), a n-by-1 or 1-by-n of doubles.
    if (iTypeSearch)
    {
        if (pDblSearch->getCols() == 1)
        {
            iRowsOcc--;
        }
        else
        {
            iColsOcc--;
        }
    }

    if ((iRowsOcc * iColsOcc) < 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of length > %d expected.\n"), "dsearch", 2, iTypeSearch);
        return types::Function::Error;
    }

    pDblInd = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());

    if (_iRetCount > 1)
    {
        pDblOcc = new types::Double(iRowsOcc, iColsOcc);
        pdOcc = pDblOcc->get();
    }

    if (iTypeSearch) // c
    {
        vDsearchC(pDblIn->get(), pDblIn->getSize(), pDblSearch->get(), pDblSearch->getSize() - 1, pDblInd->get(), pdOcc, &dInfo);
    }
    else
    {
        vDsearchD(pDblIn->get(), pDblIn->getSize(), pDblSearch->get(), pDblSearch->getSize(), pDblInd->get(), pdOcc, &dInfo);
    }

    /***** set result *****/
    out.push_back(pDblInd);

    if (_iRetCount > 1)
    {
        out.push_back(pDblOcc);
    }

    if (_iRetCount > 2)
    {
        out.push_back(new types::Double(dInfo));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
