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
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "os_string.h"
#include "os_wcstok.h"
#include <stdio.h>
}

static char* pstState = NULL;
static /* const */ char EOS = 0;

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
        // set an empty string
        pstToken = &EOS;
    }
    else
    {
        pstToken = os_strtok(pstString, pstSeps, &pstState);
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

