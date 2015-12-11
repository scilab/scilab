/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>

#include "gw_scicos.hxx"
#include "context.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_validvar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iValid = 0;
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "validvar", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "validvar", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "validvar", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(202, _("%s: Wrong size for argument #%d: string expected.\n"), "validvar", 1);
        return types::Function::Error;
    }


    if (symbol::Context::getInstance()->isValidVariableName(pS->get(0)))
    {
        iValid = 1;
    }

    out.push_back(new types::Bool(iValid));
    return types::Function::OK;
}