/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "MALLOC.h"
#include "os_wcsdup.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_strstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    types::String* pString      = NULL;
    types::String* pStrSample   = NULL;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strstr", 2);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strstr", 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strstr", 1);
        return types::Function::Error;
    }
    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strstr", 2);
        return types::Function::Error;
    }

    pString     = in[0]->getAs<types::String>();
    pStrSample  = in[1]->getAs<types::String>();

    if (pString->getSize() == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty matrix of strings expected.\n"), "strstr", 1);
        return types::Function::Error;
    }

    if (pStrSample->getSize() == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty matrix of strings expected.\n"), "strstr", 2);
        return types::Function::Error;
    }

    if (pString->getSize() != pStrSample->getSize() && pStrSample->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "strstr", 2);
        return types::Function::Error;
    }

    pOutString  = new types::String(pString->getDims(), pString->getDimsArray());

    int j = 0; /* Input parameter two is dimension one */
    for (int i = 0 ; i < pString->getSize() ; i++)
    {
        if (pStrSample->isScalar() == false)
        {
            j = i; /* Input parameter One & two have same dimension */
        }

        if (wcslen(pString->get(i)) < wcslen(pStrSample->get(j)))
        {
            pOutString->set(i, os_wcsdup(L""));
        }
        else
        {
            wchar_t* ptrwstrstr = wcsstr(pString->get(i), pStrSample->get(j));

            if (ptrwstrstr)
            {
                pOutString->set(i, os_wcsdup(ptrwstrstr));
                if (pOutString->get(i) == NULL)
                {
                    delete pOutString;
                    FREE(ptrwstrstr);
                    Scierror(999, _("%s: No more memory.\n"), "strstr");
                    return types::Function::Error;
                }
            }
            else
            {
                pOutString->set(i, os_wcsdup(L""));
                if (pOutString->get(i) == NULL)
                {
                    delete pOutString;
                    FREE(ptrwstrstr);
                    Scierror(999, _("%s: No more memory.\n"), "strstr");
                    return types::Function::Error;
                }
            }
        }
    }

    out.push_back(pOutString);
    return types::Function::OK;
}

