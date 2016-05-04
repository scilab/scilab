/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
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


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "tokens.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}

#include <sciprint.h>

types::Function::ReturnValue sci_tokens(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    types::String* pString      = NULL;
    types::String* pCharSample  = NULL;
    wchar_t* seps               = NULL;
    int sizeSeps                = 0;

    if (in.size() > 2 || in.size() == 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "tokens", 1, 2);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "tokens", 1);
        return types::Function::Error;
    }

    // first arg
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "tokens", 1);
        return types::Function::Error;
    }
    pString = in[0]->getAs<types::String>();
    if (pString->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "tokens", 1);
        return types::Function::Error;
    }
    if (wcslen(pString->get(0)) == 0)
    {
        types::Double* pOutDouble = types::Double::Empty();
        out.push_back(pOutDouble);
        return types::Function::OK;
    }

    // second arg
    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "tokens", 2);
            return types::Function::Error;
        }
        pCharSample = in[1]->getAs<types::String>();

        if (pCharSample->getSize() == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "tokens", 2);
            return types::Function::Error;
        }
        sizeSeps = pCharSample->getSize();
        seps = (wchar_t*)MALLOC((sizeSeps + 1) * sizeof(wchar_t));
        for (int i = 0; i < sizeSeps ; i++)
        {
            int iLen = (int)wcslen(pCharSample->get(i));
            if (iLen > 1 || iLen < 0)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Char(s) expected.\n"), "tokens", 2);
                delete pOutString;
                FREE(seps);
                return types::Function::Error;
            }
            seps[i] = pCharSample->get(i)[0];
        }
    }
    else // default delimiters are ' ' and Tabulation
    {
        sizeSeps = 2;
        seps = (wchar_t*)MALLOC((sizeSeps + 1) * sizeof(wchar_t));
        seps[0] = L' ';
        seps[1] = L'\t';
    }
    seps[sizeSeps] = L'\0';

    // perfom operation
    int dimsArray[2] = {0, 1};
    int dims = 2;

    wchar_t** Output_Strings = stringTokens(pString->get(0), seps, &dimsArray[0]);
    FREE(seps);
    if (Output_Strings == NULL)
    {
        //return empty matrix
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }
    else
    {
        pOutString  = new types::String(dims, dimsArray);
        pOutString->set(Output_Strings);

        for (int i = 0 ; i < dimsArray[0] ; i++)
        {
            FREE(Output_Strings[i]);
        }
        FREE(Output_Strings);
    }

    out.push_back(pOutString);
    return types::Function::OK;
}

