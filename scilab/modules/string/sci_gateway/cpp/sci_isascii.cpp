/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "BOOL.h"
#include "Scierror.h"
#include "localization.h"
#include "isascii.h"
}

types::Function::ReturnValue sci_isascii(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Bool* pBOut = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "isascii", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "isascii", 1);
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        types::String* pIn = in[0]->getAs<types::String>();
        if (pIn->getSize() == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        // concatenate all strings
        std::wstring s(pIn->get(0));
        for (int i = 1; i < pIn->getSize(); i++)
        {
            s += std::wstring(pIn->get(i));
        }

        int iResultSize = 0;
        int* pbResult = (int*)isasciiStringW((wchar_t*)s.c_str(), &iResultSize);
        if (pbResult == NULL)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        pBOut = new types::Bool(1, iResultSize);
        pBOut->set(pbResult);
        FREE(pbResult);
        out.push_back(pBOut);
    }
    else if (in[0]->isDouble())
    {
        types::Double* pIn = in[0]->getAs<types::Double>();
        int* pbResult = (int*)isasciiMatrix(pIn->get(), pIn->getSize());
        if (pbResult == NULL)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        pBOut = new types::Bool(1, pIn->getSize());
        pBOut->set(pbResult);
        FREE(pbResult);
        out.push_back(pBOut);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string or matrix expected.\n"), "isascii", 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*-------------------------------------------------------------------------------------*/
