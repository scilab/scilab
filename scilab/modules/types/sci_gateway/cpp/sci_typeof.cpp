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

#include "types.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C" {
    #include "Scierror.h"
    #include "localization.h"
}

#define OPTION "overload"

using namespace types;

Function::ReturnValue sci_typeof(typed_list &in, int _piRetCount, typed_list &out)
{
    if(in.size() < 1 || in.size() > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "typeof", 1, 2);
        return Function::Error;
    }

    // Old typeof call
    if(in.size() == 1)
    {
        out.push_back(new String(in[0]->getTypeStr().c_str()));
        return Function::OK;
    }

    // Check second argument is a string
    if(in[1]->isString() == false || in[1]->getAsString()->size_get() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "typeof", 2);
        return Function::Error;
    }

    if(strcmp(in[1]->getAsString()->string_get(0,0), OPTION) == 0)
    {
       out.push_back(new String(in[0]->getShortTypeStr().c_str()));
       return Function::OK;
    }
    else
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: '%s' expected .\n"), "typeof", 2, OPTION);
        return Function::Error;
    }

    return Function::Error;

}
