/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/*
 * Provides is_dir to scilab
 *
 */
/*--------------------------------------------------------------------------*/
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "isdir.h"
}

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_isdir(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"isdir" , 1);
        return Function::Error;
    }

    if(_iRetCount != 1)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d expected.\n"), L"isdir", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix of strings expected.\n"), L"isdir", 1);
        return Function::Error;
    }

    String* pS  = in[0]->getAsString();
    Bool* pOut  = new Bool(pS->rows_get(), pS->cols_get());

    for(int i = 0 ; i < pS->size_get() ; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pS->string_get(i));
		if(expandedPath)
		{
			pOut->bool_get()[i] = isdirW(expandedPath);
			FREE(expandedPath);
			expandedPath = NULL;
		}
		else
		{
			pOut->bool_get()[i] = FALSE;
		}
	}

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
