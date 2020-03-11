/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
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

/* desc : Let  s[k]  stands for the  k  character of Input_StringMatrixings
  ( or the  white space character if  k >length(s) ).
  part  returns  c , a matrix of character Input_StringMatrixings, such that
  c(i,j)  is the Input_StringMatrixing  "s[v(1)]...s[v(n)]"  (  s=mp(i,j)  ).
                                                                          */
/*------------------------------------------------------------------------*/
#include "double.hxx"
#include "funcmanager.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "string.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "partfunction.h"
#include <stdio.h>
#include <string.h>
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_part(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "part", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "part", 1);
        return types::Function::Error;
    }

    //part([], ...
    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::String* pS = in[0]->getAs<types::String>();

    if (in[1]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[1]->getShortTypeStr() + L"_part";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* pD = in[1]->getAs<types::Double>();
    if (pD->isVector() == false && pD->isEmpty() == false)
    {
        //non vector
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), "part", 2);
        return types::Function::Error;
    }

    size_t i_len = pD->getSize();
    std::vector<int> index(i_len);
    for (int i = 0; i < i_len; i++)
    {
        int idx = static_cast<int>(pD->get()[i]);
        if (idx < 1)
        {
            Scierror(36, _("%s: Wrong values for input argument #%d: Must be >= 1.\n"), "part", 2);
            return types::Function::Error;
        }

        index[i] = idx;
    }

    types::String* pOut = new types::String(pS->getRows(), pS->getCols());
    std::wstring string_in;

    // allocate the output strings
    std::wstring string_out(i_len, L' ');
    for (int i = 0; i < pS->getSize(); ++i)
    {
        pOut->set(i, string_out.data());
    }

    // part() algorithm
    for (int i = 0; i < pS->getSize(); ++i)
    {
        wchar_t* wcs_in = pS->get()[i];
        wchar_t* wcs_out = pOut->get()[i];
        size_t s_len = wcslen(wcs_in);

        for (int j = 0; j < i_len; ++j)
        {
            if (index[j] > s_len)
            {
                continue;
            }

            wcs_out[j] = wcs_in[index[j] - 1];
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
