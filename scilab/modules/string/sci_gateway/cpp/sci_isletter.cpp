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
#include "bool.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "isletter.h"
#include "BOOL.h"
}


types::Function::ReturnValue sci_isletter(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Bool* pOutBool   = NULL;
    types::String* pString  = NULL;
    int dimsArray[2]        = {1,0};
    int dims                = 2;
	BOOL *values            = NULL;

    if(in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "isletter", 1);
        return types::Function::Error;
    }
    if(_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isletter", 1);
        return types::Function::Error;
    }
	if(in[0]->isString() == false)
	{
		Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "isletter", 1);
		return types::Function::Error;
	}

    pString = in[0]->getAs<types::String>();
    if(pString->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "isletter", 1);
        return types::Function::Error;
    }
    if(pString->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    values = isletterW(pString->get(0), &dimsArray[1]);

    if(dimsArray[1] > 0)
    {
        pOutBool  = new types::Bool(dims, dimsArray);
        pOutBool->set((int *) values);
        out.push_back(pOutBool);
    }
    else
    {
        out.push_back(types::Double::Empty());
    }

    return types::Function::OK;
}

