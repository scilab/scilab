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
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "os_string.h"
#include <stdio.h>
}

static char* pstState = NULL;

types::Function::ReturnValue sci_strtok(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    char* pstString             = NULL;
    char* pstSeps               = NULL;
    int dims                    = 2;
    int dimsArray[2]            = {1, 1};

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "strtok", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strtok", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strtok", 1);
        return types::Function::Error;
    }

    if (in.size() == 2 && (in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strtok", 2);
        return types::Function::Error;
    }


    if (in.size() == 1)
    {
        pstSeps = in[0]->getAs<types::String>()->get(0);
    }
    else
    {
        pstString = in[0]->getAs<types::String>()->get(0);
        pstSeps = in[1]->getAs<types::String>()->get(0);
        pstState = NULL;
        if (strlen(pstString) == 0)
        {
            pOutString  = new types::String(dims, dimsArray);
            pOutString->set(0, "");
            out.push_back(pOutString);
            return types::Function::OK;
        }
    }

    char* pstToken = NULL;
    if (pstString == NULL && pstState == NULL)
    {
        pstToken = "";
    }
    else
    {
#ifndef _MSC_VER
        pstToken = strtok(pstString, pstSeps, &pstState);
#else
        pstToken = strtok_s(pstString, pstSeps, &pstState);
#endif
    }

    pOutString  = new types::String(dims, dimsArray);

    if (pstToken)
    {
        pOutString->set(0, pstToken);
    }
    else
    {
        pOutString->set(0, "");
    }

    out.push_back(pOutString);
    return types::Function::OK;
}

