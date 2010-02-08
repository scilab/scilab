/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "funcmanager.hxx"
#include "context.hxx"
#include "core_gw.hxx"
#include "types.hxx"

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_getmodules(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	InternalType *pIT	= symbol::Context::getInstance()->get(symbol::Symbol("module_list"));

	out.push_back(pIT);
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
