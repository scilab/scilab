/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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