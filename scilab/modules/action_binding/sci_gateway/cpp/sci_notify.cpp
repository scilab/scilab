/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
#include "function.hxx"
#include "action_binding_GiwsExports.hxx"
#include "action_binding_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_notify(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString  = NULL;
    wchar_t* wcsInput       = NULL;

    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"notify" , 1);
        return types::Function::Error;
    }
    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"notify", 1);
        return types::Function::Error;
    }
    pString = in[0]->getAs<types::String>();

    if(pString->isScalar() == FALSE)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d: A string expected.\n"), L"notify" , 1);
        return types::Function::Error;
    }
    wcsInput = pString->get(0);

    char* strInput = wide_string_to_UTF8(wcsInput);
    Signal_notify(getScilabJavaVM(), strInput);
    FREE(strInput);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
