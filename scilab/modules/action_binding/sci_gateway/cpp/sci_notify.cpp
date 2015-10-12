/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "function.hxx"
#include "action_binding_gw.hxx"
#include "string.hxx"
#include "Signal.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_notify(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString  = NULL;
    wchar_t* wcsInput       = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "notify" , 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "notify", 1);
        return types::Function::Error;
    }
    pString = in[0]->getAs<types::String>();

    if (pString->isScalar() == FALSE)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "notify" , 1);
        return types::Function::Error;
    }
    wcsInput = pString->get(0);

    char* strInput = wide_string_to_UTF8(wcsInput);
    try
    {
        org_scilab_modules_action_binding_utils::Signal::notify(getScilabJavaVM(), strInput);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), "notify", e.whatStr().c_str());
        FREE(strInput);
        return types::Function::Error;
    }
    FREE(strInput);

    return types::Function::OK;
}

/*--------------------------------------------------------------------------*/
