/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "jvm_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include "system_setproperty.h"
#include "Scierror.h"
#include "localization.h"
#include "configvariable_interface.h"

}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_system_setproperty(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at %d expected.\n"), "system_setproperty", 2);
        return types::Function::Error;
    }

    if (_piRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "system_setproperty", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "system_setproperty" , 1);
        return types::Function::Error;
    }

    if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "system_setproperty" , 2);
        return types::Function::Error;
    }

    char* pstProperty        = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));
    char* pstValue           = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
    char* pstPreviousValue   = system_setproperty(pstProperty, pstValue);

    FREE(pstProperty);
    FREE(pstValue);

    if (pstPreviousValue)
    {
        types::String* pSPreviousValue = new types::String(pstPreviousValue);
        out.push_back(pSPreviousValue);
        FREE(pstPreviousValue);
    }
    else
    {
        types::Double* pDPreviousValue = types::Double::Empty();
        out.push_back(pDPreviousValue);
    }
    return types::Function::OK;

}
/*--------------------------------------------------------------------------*/
