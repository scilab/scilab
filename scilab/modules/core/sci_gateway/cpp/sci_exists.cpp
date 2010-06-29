/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

enum ScopeRange {
    Local,
    NoLocal,
    All,
    UnknownRange
};

enum PrivateResult {
    VariableFound,
    VariableNotFound,
    FunctionFailed
};

static ScopeRange getScopeFromOption(const char *_psScope)
{
    if (strcmp(_psScope, "all") == 0 || strcmp(_psScope, "a") == 0)
    {
        return All;
    }
    if (strcmp(_psScope, "local") == 0 || strcmp(_psScope, "l") == 0)
    {
        return Local;
    }
    if (strcmp(_psScope, "nolocal") == 0 || strcmp(_psScope, "n") == 0)
    {
        return NoLocal;
    }

    return UnknownRange;
}

static PrivateResult sci_existsOrIsdef(types::typed_list &in, const char *fname)
{
    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), fname, 1, 2);
        return FunctionFailed;
    }

    if (!in[0]->isString() || in[0]->getAsString()->size_get() != 1)
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A single string expected.\n"), fname, 1);
        return FunctionFailed;
    }

    if (in.size() == 2 && (!in[1]->isString() || in[1]->getAsString()->size_get() != 1))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A single string expected.\n"), fname, 2);
        return FunctionFailed;
    }

    const char *psScope = NULL;
    if (in.size() == 2)
    {
        psScope = in[1]->getAsString()->string_get(0,0);
    }
    else
    {
        // Default option is "all"
        psScope = "all";
    }


    types::InternalType *pitReturn = NULL;
    switch (getScopeFromOption(psScope))
    {
    case All:
        pitReturn = symbol::Context::getInstance()->get(in[0]->getAsString()->string_get(0,0));
        break;
    case Local:
        pitReturn = symbol::Context::getInstance()->getCurrentLevel(in[0]->getAsString()->string_get(0,0));
        break;
    case NoLocal:
        pitReturn = symbol::Context::getInstance()->getAllButCurrentLevel(in[0]->getAsString()->string_get(0,0));
        break;
    default :
        Scierror(36, _("%s: Wrong input argument %d.\n"), fname, 2);
        return FunctionFailed;
    }


    if (pitReturn != NULL)
    {
        return VariableFound;
    }
    else
    {
        return VariableNotFound;
    }
}

types::Function::ReturnValue sci_exists(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    switch (sci_existsOrIsdef(in, "exists"))
    {
    case VariableFound :
        out.push_back(new types::Double(1.0));
        return types::Function::OK;
    case VariableNotFound :
        out.push_back(new types::Double(0.0));
        return types::Function::OK;
    default :
        return types::Function::Error;
    }
}

types::Function::ReturnValue sci_isdef(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    switch (sci_existsOrIsdef(in, "isdef"))
    {
    case VariableFound :
        out.push_back(new types::Bool(true));
        return types::Function::OK;
    case VariableNotFound :
        out.push_back(new types::Bool(false));
        return types::Function::OK;
    default :
        return types::Function::Error;
    }
}
