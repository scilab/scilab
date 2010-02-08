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
Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	ast::Exp* pExp = NULL;
	if(in.size() < 1 || in.size() > 3)
	{
		return Function::Error;
	}

	if(in[0]->getType() == InternalType::RealString)
	{//1st argument is a path, parse file and execute it
		char pstParsePath[PATH_MAX + FILENAME_MAX];
		int iParsePathLen		= 0;
		String* pS = in[0]->getAsString();
		if(pS->size_get() != 1)
		{
			return Function::Error;
		}

		char* pstFile = pS->string_get(0);
		C2F(cluni0)(pstFile, pstParsePath, &iParsePathLen, (long)strlen(pstFile), PATH_MAX + FILENAME_MAX);
		Parser::getInstance()->parseFile(pstParsePath, "exec");
		if(Parser::getInstance()->getExitStatus() !=  Parser::Succeded)
		{
			return Function::Error;
		}

		pExp = Parser::getInstance()->getTree();
	}
	else if(in[0]->getType() == InternalType::RealMacro)
	{//1st argument is a macro name, execute it in the current environnement
		pExp = in[0]->getAsMacro()->body_get();
	}
	else if(in[0]->getType() == InternalType::RealMacroFile)
	{//1st argument is a macro name, parse and execute it in the current environnement
		if(in[0]->getAsMacroFile()->parse() == false)
		{
			return Function::Error;
		}
		pExp = in[0]->getAsMacroFile()->macro_get()->body_get();
	}
	else
	{
		return Function::Error;
	}

	try
	{
		//excecute script
		ast::ExecVisitor execMe;
		pExp->accept(execMe);
	}
	catch(std::string st)
	{
	}

	return Function::OK;
}
/*--------------------------------------------------------------------------*/
