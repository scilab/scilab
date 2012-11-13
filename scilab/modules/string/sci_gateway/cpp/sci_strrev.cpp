/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "stringsstrrev.h"
}

types::Function::ReturnValue sci_strrev(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString      = NULL; 
    types::String* pOutString   = NULL; 
    wchar_t **OutputStrings     = NULL;
    
    if(in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strrev", 1);
        return types::Function::Error;
    }    
    if(_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strrev", 1);
        return types::Function::Error;
    }
	if(in[0]->isString() == false)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of Strings expected.\n"), "strrev", 1);
		return types::Function::Error;
	}

    pString = in[0]->getAs<types::String>();
   // wchar_t* pwstStr = in[0]->getAs<types::String>()->get(0);
   
    OutputStrings = strings_strrev(pString->get(),pString->getSize());
    
    pOutString = new types::String(pString->getDims(),pString->getDimsArray());
    pOutString->set(OutputStrings);
    
    out.push_back(pOutString);
    return types::Function::OK;
}

