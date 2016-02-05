/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

