/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"prompt", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0) /* Get current Scilab prompt */
    {
        if (_iRetCount > 2)
        {
            ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"prompt", 1, 2);
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
            ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"prompt", 1);
            return types::Function::Error;
        }

        if ((in[0]->isString() == false) || !in[0]->getAs<types::String>()->isScalar())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"prompt", 1);
            return types::Function::Error;
        }
        SetTemporaryPrompt(wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0)));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
