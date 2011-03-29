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
}


types::Function::ReturnValue sci_strcmp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOutString   = NULL;
    types::String* pString      = NULL;
    types::String* pStrSample   = NULL;
       
    if(in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"strcmp", 2);
        return types::Function::Error;
    }    
    if(_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"strcmp", 1);
        return types::Function::Error;
    }
	if(in[0]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"strcmp", 1);
		return types::Function::Error;
	}
	if(in[1]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"strcmp", 2);
		return types::Function::Error;
	}

    pString     = in[0]->getAs<types::String>();
    pStrSample  = in[1]->getAs<types::String>();

  
    
    if(pString->getSize() != pStrSample->getSize() && pStrSample->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d.\n"),L"strcmp", 2);
        return types::Function::Error;
    }
    
    pOutString  = new types::String(pString->getDims(), pString->getDimsArray());


    
    out.push_back(pOutString);
    return types::Function::OK;
}

