/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "funcmanager.hxx"
#include "context.hxx"

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_whereis(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	InternalType *pIT = NULL;
	std::string stModule;

	/* Check the number of input argument */
	if(in.size() != 1)
	{
		return Function::Error;
	}

	/* Check the number of output argument */
	if(_iRetCount != 1)
	{
		return Function::Error;
	}

	if(in[0]->getType() == InternalType::RealString)
	{
		types::String* pS = in[0]->getAsString();

		if(pS->size_get() != 1)
		{
			return Function::Error;
		}

		char* pstFuncName = pS->string_get(0);
		symbol::Context* pContext = symbol::Context::getInstance();

		string funcName(pstFuncName);
		pIT = pContext->get_fun(funcName);
		if(pIT == NULL)
		{
			return Function::Error;
		}
	}
	else
	{
		pIT = in[0];
	}

	switch(pIT->getType())
	{
	case InternalType::RealFunction :
	case InternalType::RealMacro :
	case InternalType::RealMacroFile :
		stModule = pIT->getAsCallable()->getModule();
		break;
	default :
		return Function::Error;
	}

	out.push_back(new types::String(stModule.c_str()));
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
