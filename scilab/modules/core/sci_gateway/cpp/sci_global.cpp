/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "context.hxx"
#include "string.hxx"
#include "core_gw.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

#define FORBIDDEN_CHARS L" */\\.,;:^@><!=+-&|()~\n\t'\""

types::Function::ReturnValue sci_global(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //check input arguments
    for (int i = 0 ; i < in.size() ; i++)
    {
        if (in[i]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "global", i + 1);
            return types::Function::Error;
        }

        if (in[i]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "global", i + 1);
            return types::Function::Error;
        }
    }

    //check output arguments
    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: At most %d expected.\n"), "global", 1);
        return types::Function::Error;
    }

    symbol::Context* pCtx = symbol::Context::getInstance();

    for (int i = 0 ; i < in.size() ; i++)
    {
        wchar_t* wcsVarName = in[i]->getAs<types::String>()->get(0);
        if (symbol::Context::getInstance()->isValidVariableName(wcsVarName) == false)
        {
            char* pstrVarName = wide_string_to_UTF8(wcsVarName);
            Scierror(999, _("%s : Wrong value for argument #%d: %s\n"), "global", i + 1, pstrVarName);
            FREE(pstrVarName);
            return types::Function::Error;
        }

        symbol::Symbol pstVar(symbol::Symbol(const_cast<wchar_t*>(wcsVarName)));

        if (pCtx->isGlobalVisible(pstVar))
        {
            continue;
        }

        types::InternalType* pIT = NULL;

        if (pCtx->isGlobal(pstVar))
        {
            pIT = pCtx->getGlobalValue(pstVar);
        }
        else
        {
            pIT = pCtx->getCurrentLevel(pstVar);
            pCtx->setGlobal(pstVar);
        }

        //set global variable visible in current scope
        pCtx->setGlobalVisible(pstVar, true);

        if (pIT)
        {
            //assign local value to new global variable
            pCtx->put(pstVar, pIT);
        }
        else
        {
            pCtx->put(pstVar, types::Double::Empty());
        }

    }
    return types::Function::OK;
}
