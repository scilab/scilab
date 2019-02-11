/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2018 - Stéphane Mottelet
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

#include <cfloat> // DBL_MAX
#include <cmath> // std::nextafter
#include <complex>
#include "string.hxx"
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_nearfloat(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double dblMode;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "nearfloat", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "nearfloat", 1);
        return types::Function::Error;
    }

    //first arg
    types::InternalType* pIT = in[0];
    if (pIT->isString())
    {
        types::String* pStr = pIT->getAs<types::String>();

        if (pStr->getSize() > 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "nearfloat", 1);
            return types::Function::Error;
        }

        wchar_t* wcsString = pStr->get(0);
        if (wcscmp(wcsString, L"succ") == 0)
        {
            dblMode = std::numeric_limits<double>::infinity();
        }
        else if (wcscmp(wcsString, L"pred") == 0)
        {
            dblMode = -std::numeric_limits<double>::infinity();
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "nearfloat", 1, "\"succ\",\"pred\"");
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "nearfloat", 1);
        return types::Function::Error;
    }

    //second arg
    pIT = in[1];
    if (pIT->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + pIT->getShortTypeStr() + L"_nearfloat";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }


    types::Double* pDblIn = pIT->getAs<types::Double>();
    if (pDblIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "nearfloat", 2);
        return types::Function::Error;
    }

    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
    double *pdRealIn = pDblIn->get();
    double *pdRealOut = pDblOut->get();
    for (int i = 0; i < pDblIn->getSize(); ++i)
    {
        pdRealOut[i] = std::nextafter(pdRealIn[i], dblMode);
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
