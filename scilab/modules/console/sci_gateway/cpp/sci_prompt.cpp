/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "console_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "prompt.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_prompt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char currentPrompt[PROMPT_SIZE_MAX];

    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "prompt", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0) /* Get current Scilab prompt */
    {
        if (_iRetCount > 2)
        {
            Scierror(999, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "prompt", 1, 2);
            return types::Function::Error;
        }

        GetCurrentPrompt(currentPrompt);
        out.push_back(new types::String(currentPrompt));
        if (_iRetCount == 2)
        {
            types::Double* pDbl = new types::Double(1, 1);
            pDbl->set(0, ConfigVariable::getPauseLevel());
            out.push_back(pDbl);
        }
    }
    else //if(in.size() == 1)
    {
        if (_iRetCount > 1)
        {
            Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "prompt", 1);
            return types::Function::Error;
        }

        if ((in[0]->isString() == false) || !in[0]->getAs<types::String>()->isScalar())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "prompt", 1);
            return types::Function::Error;
        }

        char* pstrPrompt = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));
        if (strlen(pstrPrompt) > PROMPT_SIZE_MAX - 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Single string of size %d expected.\n"), "prompt", 1, PROMPT_SIZE_MAX - 1);
            FREE(pstrPrompt);
            return types::Function::Error;
        }

        SetTemporaryPrompt(pstrPrompt);
        FREE(pstrPrompt);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
