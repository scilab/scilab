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

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "FindFileAssociation.h"
#include "PATH_MAX.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "findfileassociation";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_findfileassociation(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pS = nullptr;
    std::wstring param1;
    std::wstring param2(L"open");

    int rhs = static_cast<int>(in.size());
    if (rhs != 1 && rhs != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname.data(), 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
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

    if (rhs == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 2);
            return types::Function::Error;
        }

        pS = in[1]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 2);
            return types::Function::Error;
        }

        param2 = pS->get()[0];
    }

    wchar_t* output = FindFileAssociation(param1.data(), param2.data()) ;
    if (output)
    {
        out.push_back(new types::String(output));
    }
    else
    {
        out.push_back(types::Double::Empty());
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
