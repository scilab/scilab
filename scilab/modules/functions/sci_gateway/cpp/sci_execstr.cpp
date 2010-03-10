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
#include "mutevisitor.hxx"
#include "yaspio.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "MALLOC.h"

#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
#include "prompt.h"
}


using namespace std;
using namespace types;
using namespace ast;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_execstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	bool bErrCatch		= false;
	char* pstMsg			= NULL;
	Exp* pExp					= NULL;
	char *pstCommand	= NULL;

	if(in.size() < 1 || in.size() > 3)
	{
		return Function::Error;
	}

	if(in.size() > 1)
	{
		if(in[1]->getType() == InternalType::RealString)
		{//errcatch
			String* pS = in[1]->getAsString();
			if(pS->size_get() != 1)
			{
				return Function::Error;
			}
			
			if(stricmp(pS->string_get(0), "errcatch") == 0)
			{
				bErrCatch = true;
			}
			else
			{
				char stErr[1024];
#ifdef _MSC_VER
				sprintf_s(stErr, 1024, "\"%s\" value is not a valid value for exec function", pS->string_get(0));
#else
				sprintf(stErr, "\"%s\" value is not a valid value for exec function", pS->string_get(0));
#endif
				YaspWrite(stErr);
				return Function::Error;
			}
		}
		else
		{//not managed
			YaspWrite("Bad 2nd parameter type in execstr call");
			return Function::Error;
		}

		//3rd parameter
		if(in.size() == 3)
		{
			if(in[2]->getType() == InternalType::RealString)
			{
				String* pS = in[1]->getAsString();
				if(pS->size_get() != 1)
				{
					return Function::Error;
				}
			
				pstMsg = pS->string_get(0);
			}
			else
			{//not managed
				YaspWrite("Bad 3rd parameter type in execstr call");
				return Function::Error;
			}
		}
	}

	if(in[0]->getType() == InternalType::RealString)
	{//1st argument is a scilab source code, parse and execute it
		String* pS = in[0]->getAsString();

		//must be a vector
		if(pS->rows_get() != 1 && pS->cols_get() != 1)
		{
			return Function::Error;
		}

		int iTotalLen = pS->size_get(); //add \n after each string
		for(int i = 0 ; i < pS->size_get() ; i++)
		{
			iTotalLen += (int)strlen(pS->string_get(i));
		}

		pstCommand = (char*)MALLOC(sizeof(char) * (iTotalLen + 1));//+1 for null termination

		int iPos = 0;
		for(int i = 0 ; i < pS->size_get() ; i++)
		{
			strcpy(pstCommand + iPos, pS->string_get(i));
			iPos = (int)strlen(pstCommand);
			pstCommand[iPos++] = '\n';
			pstCommand[iPos] = 0;
		}

		Parser::getInstance()->parse(pstCommand);
		if(Parser::getInstance()->getExitStatus() !=  Parser::Succeded)
		{
			YaspWrite(Parser::getInstance()->getErrorMessage());
			FREE(pstCommand);
			return Function::Error;
		}

		pExp = Parser::getInstance()->getTree();
	}
	else
	{//not managed
		YaspWrite("Bad 1st parameter type in execstr call");
		return Function::Error;
	}

	if(pExp == NULL)
	{
		return Function::Error;
	}

	MuteVisitor mute;
	pExp->accept(mute);

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
			//excecute script
			ExecVisitor execMe;
			(*j)->accept(execMe);
		}
		catch(std::string st)
		{
			//print error
			YaspWrite(const_cast<char*>(pstCommand));
			YaspWrite("\n");
			YaspWrite(const_cast<char*>(st.c_str()));
			return Function::Error;
		}
	}

	Parser::getInstance()->freeTree();
	FREE(pstCommand);
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
