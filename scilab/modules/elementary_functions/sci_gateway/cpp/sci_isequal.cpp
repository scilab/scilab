/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "elem_func_gw.hxx"
#include "funcmanager.hxx"
#include "context.hxx"

using namespace types;

Function::ReturnValue sci_isequal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	bool bResult = true;
	if(in.size() <= 1 || _iRetCount != 1)
	{
		return Function::Error;
	}

	for(int i = 1 ; i < in.size() ; i++)
	{
		InternalType* pIn1 = in[i - 1];
		InternalType* pIn2 = in[i];

		if(*pIn1 != *pIn2)
		{
			bResult = false;
		}
	}

	Bool* pBool = new Bool(bResult);

	out.push_back(pBool);
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
