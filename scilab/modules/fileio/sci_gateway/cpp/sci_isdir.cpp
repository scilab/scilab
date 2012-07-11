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
#include "fileio_gw.hxx"
#include "bool.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "isdir.h"
}

using namespace types;

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

    String* pS  = in[0]->getAs<types::String>();
    Bool* pOut  = new Bool(pS->getRows(), pS->getCols());

    for(int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pS->get(i));
		if(expandedPath)
		{
			pOut->get()[i] = isdirW(expandedPath);
			FREE(expandedPath);
			expandedPath = NULL;
		}
		else
		{
			pOut->get()[i] = FALSE;
		}
	}

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
