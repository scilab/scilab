/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "registry.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
#pragma comment(lib, "winmm.lib")
/*--------------------------------------------------------------------------*/
const std::string fname = "mcisendstring";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mcisendstring(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring param1;
    types::String* pS = nullptr;

    int out2 = 0;
    std::wstring out3(L"OK");

    wchar_t output[2048];

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount < 1 || _iRetCount > 3)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), fname.data(), 1, 3);
        return types::Function::Error;
    }


    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    pS = in[0]->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    param1 = pS->get()[0];
    char* test = wide_string_to_UTF8(param1.data());
    char tout[2048];
    MCIERROR err = mciSendString(test, tout, sizeof(tout), NULL);
    out2 = (int)err;
    if (err)
    {
        wchar_t errtxt[128];

        if (mciGetErrorStringW(err, errtxt, 128) == FALSE)
        {
            os_swprintf(errtxt, L"%ls", L"Unknown MCI error");
        }

        out3 = errtxt;

        out.push_back(types::Bool::False());
    }
    else
    {
        out.push_back(types::Bool::True());
    }

    if (_iRetCount > 1)
    {
        out.push_back(new types::Double(out2));
    }

    if (_iRetCount > 2)
    {
        out.push_back(new types::String(out3.data()));
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
