/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    std::string param1;
    types::String* pS = nullptr;

    int out2 = 0;
    std::string out3("OK");

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
    char tout[2048];
    MCIERROR err = mciSendString(param1.data(), tout, sizeof(tout), NULL);
    out2 = (int)err;
    if (err)
    {
        char errtxt[128];

        if (mciGetErrorStringA(err, errtxt, 128) == FALSE)
        {
            os_sprintf(errtxt, "%s", "Unknown MCI error");
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
