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
#include "yaspio.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
#include "prompt.h"
}


#define EXEC_MODE_SILENT		0xFFFF
#define EXEC_MODE_VERBOSE		0x0001
#define EXEC_MODE_PROMPT		0x0002

using namespace types;
using namespace ast;
using namespace std;

bool checkPrompt(int _iMode, int _iCheck);
void printLine(char* _stPrompt, char* _stLine);

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	bool bErrCatch	= false;
	int iMode				= EXEC_MODE_VERBOSE;
	Exp* pExp				= NULL;


	if(in.size() < 1 || in.size() > 3)
	{
		return Function::Error;
	}

	if(in.size() > 1)
	{//errcatch or mode
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

			if(in.size() > 2)
			{
				if(in[2]->getType() == InternalType::RealDouble)
				{//mode
					Double* pD = in[2]->getAsDouble();
					if(pD->size_get() != 1 || pD->isComplex())
					{
						return Function::Error;
					}

					iMode = (int)pD->real_get()[0];
				}
			}
			else
			{
				YaspWrite("Bad 3rd parameter type in exec call");
				return Function::Error;
			}
		}
		else if(in[1]->getType() == InternalType::RealDouble)
		{//mode
			Double* pD = in[1]->getAsDouble();
			if(pD->size_get() != 1 || pD->isComplex())
			{
				return Function::Error;
			}

			iMode = (int)pD->real_get()[0];
		}
		else
		{//not managed
			YaspWrite("Bad 2nd parameter type in exec call");
			return Function::Error;
		}
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
			YaspWrite(Parser::getInstance()->getErrorMessage());
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

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

	char stPrompt[64];
	//get prompt
	GetCurrentPrompt(stPrompt);

	std::ifstream file(in[0]->getAsString()->string_get(0));

	char str[1024];
	char strLastLine[1024];
	int currentLine = -1; //no data in str
	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
			if(checkPrompt(iMode, EXEC_MODE_VERBOSE))
			{
				Exp *currentExp = (*j);
				Location loc = currentExp->location_get();
				
				//positionning file curser at loc.first_line
				{
					//strange case, current position is after the wanted position
					if(currentLine > loc.first_line)
					{
						//reset line counter and restart reading at the start of the file.
						currentLine = -1;
						file.seekg( 0, ios_base::beg );
					}

					//bypass previous lines
					for(int i = currentLine ; i < loc.first_line - 1; i++)
					{
						currentLine++;
						file.getline(str, 1024);
					}
				}

				////start is at position 1 so read the new line
				////for the first read, str is empty, so fill it
				//if(loc.first_column == 1)
				//{
				//	currentLine++;
				//	file.getline(str, 1024);
				//}

				printLine(stPrompt, str + (loc.first_column - 1));

				//print other full lines
				for(int i = loc.first_line; i < (loc.last_line - 1) ; i++)
				{
					currentLine++;
					file.getline(str, 1024);
					printLine(stPrompt, str);
				}

				if(loc.last_line > loc.first_line)
				{
					//last line
					file.getline(str, 1024);
					currentLine++;

					strncpy(strLastLine, str, loc.last_column);
					printLine(stPrompt, strLastLine);
				}
			}


			//manage mute option
			//getYaspInputMethod

			//excecute script
			ExecVisitor execMe;
			(*j)->accept(execMe);
		}
		catch(std::string st)
		{
			Function::Error;
		}
	}

	file.close();
	return Function::OK;
}

bool checkPrompt(int _iMode, int _iCheck)
{
	return ((_iMode & _iCheck) == _iCheck);
}

void printLine(char* _stPrompt, char* _stLine)
{
	//print prompt
	YaspWrite(_stPrompt);
	//print first line
	YaspWrite(_stLine);
	YaspWrite("\n");
}
/*--------------------------------------------------------------------------*/
