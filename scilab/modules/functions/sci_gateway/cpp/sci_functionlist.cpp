/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "setenvvar.hxx"
#include "execvisitor.hxx"


extern "C"
{
#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
}

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_funclist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	symbol::Context* pContext = symbol::Context::getInstance();

	wchar_t* pstLibName = NULL;
	if(in.size() > 1)
	{
		return Function::Error;
	}

	if(in.size() == 1)
	{
		InternalType* pIT = in[0];

		if(pIT->getType() != InternalType::RealString)
		{
			return Function::Error;
		}

		String *pS = pIT->getAsString();

		if(pS->size_get() != 1)
		{
			return Function::Error;
		}

		pstLibName = pS->string_get(0);
	}
	else
	{
		pstLibName = L"";
	}

	wstring libName(pstLibName);
	std::list<wstring> FuncList = pContext->get_funlist(libName);

	String *pS = new String((int)FuncList.size(), 1);

	std::list<wstring>::iterator it;
	int i = 0;
	for(it = FuncList.begin() ; it != FuncList.end() ; it++)
	{
		pS->string_set(i++, 0, it->c_str());
	}

	out.push_back(pS);
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
