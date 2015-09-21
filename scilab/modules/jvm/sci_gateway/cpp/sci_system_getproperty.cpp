/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "jvm_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include "system_getproperty.h"
#include "Scierror.h"
#include "localization.h"
#include "configvariable_interface.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_system_getproperty(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "system_getproperty", 1);
        return types::Function::Error;
    }

    if (_piRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "system_getproperty", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "system_getproperty" , 1);
        return types::Function::Error;
    }

    char* pstProperty = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));
    char* pstValue = system_getproperty(pstProperty, "unknown");
    types::String* pS = new types::String(pstValue);
    out.push_back(pS);

    FREE(pstProperty);
    FREE(pstValue);

    return types::Function::OK;
}

/*--------------------------------------------------------------------------*/

