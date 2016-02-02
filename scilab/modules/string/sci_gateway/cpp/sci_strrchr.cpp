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
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_strrchr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    types::String* pString      = NULL;
    types::String* pCharSample  = NULL;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strrchr", 2);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strrchr", 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strrchr", 1);
        return types::Function::Error;
    }
    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strrchr", 2);
        return types::Function::Error;
    }

    pString      = in[0]->getAs<types::String>();
    pCharSample  = in[1]->getAs<types::String>();

    if (pString->getSize() == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty matrix of strings expected.\n"), "strrchr", 1);
        return types::Function::Error;
    }

    if (pCharSample->getSize() == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty matrix of strings expected.\n"), "strrchr", 2);
        return types::Function::Error;
    }

    if (pString->getSize() != pCharSample->getSize() && pCharSample->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "strrchr", 2);
        return types::Function::Error;
    }

    pOutString  = new types::String(pString->getDims(), pString->getDimsArray());

    int j = 0; /* Input parameter two is dimension one */
    for (int i = 0 ; i < pString->getSize() ; i++)
    {
        if (pCharSample->isScalar() == false)
        {
            j = i; /* Input parameter One & two have same dimension */
        }
        int iLen = (int)wcslen(pCharSample->get(j));
        if (iLen != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Char(s) expected.\n"), "strrchr", 2);
            delete pOutString;
            return types::Function::Error;
        }

        if (wcslen(pString->get(i)) < wcslen(pCharSample->get(j)))
        {
            pOutString->set(i, L"");
        }
        else
        {
            wchar_t* ptrwstrstr = wcsrchr(pString->get(i), pCharSample->get(j)[0]);

            if (ptrwstrstr)
            {
                pOutString->set(i, ptrwstrstr);
                if (pOutString->get(i) == NULL)
                {
                    delete pOutString;
                    FREE(ptrwstrstr);
                    Scierror(999, _("%s: No more memory.\n"), "strrchr");
                    return types::Function::Error;
                }
            }
            else
            {
                pOutString->set(i, L"");
                if (pOutString->get(i) == NULL)
                {
                    delete pOutString;
                    Scierror(999, _("%s: No more memory.\n"), "strrchr");
                    return types::Function::Error;
                }
            }
        }
    }

    out.push_back(pOutString);
    return types::Function::OK;
}

