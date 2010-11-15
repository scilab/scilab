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
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "yaspio.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "os_wcsicmp.h"
#include "expandPathVariable.h"
#include "prompt.h"
#include "Scierror.h"
#include "localization.h"
#include "os_swprintf.h"
}


#define EXEC_MODE_MUTE			0xFFFFFFFF //-1)
#define EXEC_MODE_VERBOSE		0x00000001 //(1)
#define EXEC_MODE_PROMPT		0x00000002 //(2)

using namespace types;
using namespace ast;
using namespace std;

bool checkPrompt(int _iMode, int _iCheck);
void printLine(char* _stPrompt, char* _stLine);
void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer, int _iMode);

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iErr        = 0;
	bool bErrCatch	= false;
	int iMode		= EXEC_MODE_VERBOSE;
	Exp* pExp		= NULL;
    Parser parser;

	if(in.size() < 1 || in.size() > 3)
	{
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d to %d expected.\n"), L"exec" , 1, 3);
        return Function::Error;
	}

	if(in.size() > 1)
	{//errcatch or mode
        if(in[1]->getType() == InternalType::RealString && in[1]->getAsString() ->size_get() == 1)
		{//errcatch
			String* pS = in[1]->getAsString();
			if(os_wcsicmp(pS->string_get(0), L"errcatch") == 0)
			{
				bErrCatch = true;
			}
			else
			{
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d: 'errcatch' expected.\n"), L"execstr", 2);
				return Function::Error;
			}

			if(in.size() > 2)
			{
                if(in[2]->getType() != InternalType::RealDouble || in[2]->getAsDouble()->size_get() != 1)
				{//mode
                    ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"exec", 3);
                    return Function::Error;
				}

				iMode = (int)in[2]->getAsDouble()->real_get()[0];
			}
		}
		else if(in[1]->getType() == InternalType::RealDouble && in[1]->getAsDouble()->size_get() == 1)
        {//mode
            iMode = (int)in[1]->getAsDouble()->real_get()[0];
		}
		else
		{//not managed
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer or string expected.\n"), L"exec", 2);
            return Function::Error;
		}
	}

    if(in[0]->getType() == InternalType::RealString && in[0]->getAsString()->size_get() == 1)
	{//1st argument is a path, parse file and execute it
		int iParsePathLen		= 0;
		String* pS = in[0]->getAsString();

        wchar_t* pstFile = pS->string_get(0);
        wchar_t *expandedPath = expandPathVariableW(pstFile);
        parser.parseFile(expandedPath, L"exec");
        FREE(expandedPath);
		if(parser.getExitStatus() !=  Parser::Succeded)
		{
			YaspWriteW(parser.getErrorMessage());
			parser.freeTree();
			return Function::Error;
		}

		pExp = parser.getTree();
	}
	else if(in[0]->getType() == InternalType::RealMacro)
	{//1st argument is a macro name, execute it in the current environnement
		pExp = in[0]->getAsMacro()->body_get();
	}
	else if(in[0]->getType() == InternalType::RealMacroFile)
	{//1st argument is a macro name, parse and execute it in the current environnement
		if(in[0]->getAsMacroFile()->parse() == false)
		{
            ScierrorW(999, _W("%ls: Unable to parse macro '%s'"), "exec", in[0]->getAsMacroFile()->getName().c_str());
			return Function::Error;
		}
		pExp = in[0]->getAsMacroFile()->macro_get()->body_get();
	}
	else
	{
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"exec", 1);
		return Function::Error;
	}

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

	char stPrompt[64];
	//get prompt
	GetCurrentPrompt(stPrompt);

    wchar_t* pwstFile =  expandPathVariableW(in[0]->getAsString()->string_get(0));
    char* pstFile = wide_string_to_UTF8(pwstFile);
	std::ifstream file(pstFile);
    FREE(pstFile);
    FREE(pwstFile);

	char str[1024];
	int iCurrentLine = -1; //no data in str

    //save current prompt mode
    ConfigVariable::PromptMode oldVal = ConfigVariable::getPromptMode();

    if(bErrCatch)
    {
        ConfigVariable::setPromptMode(ConfigVariable::silent);
    }

	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
			if(checkPrompt(iMode, EXEC_MODE_MUTE))
			{
				//manage mute option
				(*j)->mute();
				MuteVisitor mute;
				(*j)->accept(mute);
			}
			else if(checkPrompt(iMode, EXEC_MODE_VERBOSE))
			{
				printExp(&file, *j, stPrompt, &iCurrentLine, str, iMode);
			}

            //excecute script
            ExecVisitor execMe;
            (*j)->accept(execMe);

            bool bImplicitCall = false;
            
            //to manage call without ()
            if(execMe.result_get() != NULL && execMe.result_get()->getAsCallable())
            {
                Callable *pCall = execMe.result_get()->getAsCallable();
                types::typed_list out;
                types::typed_list in;

                ExecVisitor execCall;
                Function::ReturnValue Ret = pCall->call(in, 1, out, &execCall);

                if(Ret == Callable::OK)
                {
                    if(out.size() == 0)
                    {
                        execMe.result_set(NULL);
                    }
                    else if(out.size() == 1)
                    {
                        out[0]->DecreaseRef();
                        execMe.result_set(out[0]);
                    }
                    else
                    {
                        for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                        {
                            out[i]->DecreaseRef();
                            execMe.result_set(i, out[i]);
                        }
                    }

                    bImplicitCall = true;
                }
                else if(Ret == Callable::Error)
                {
                    if(ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(pCall->getName());
                    }

                    if(pCall->isMacro() || pCall->isMacroFile())
                    {
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), (*j)->location_get().first_line, pCall->getName().c_str());
                        throw ScilabMessage(szError);
                    }
                    else
                    {
                        throw ScilabMessage();
                    }
                }
            }

            //update ans variable.
			if(execMe.result_get() != NULL && execMe.result_get()->isDeletable())
			{
				wstring varName = L"ans";
				symbol::Context::getInstance()->put(varName, *execMe.result_get());
				if( (*j)->is_verbose() && 
                    !checkPrompt(iMode, EXEC_MODE_MUTE) && 
                    checkPrompt(iMode, EXEC_MODE_VERBOSE) && 
                    bErrCatch == false)
				{
					std::wostringstream ostr;
					ostr << L"ans = " << std::endl;
					ostr << std::endl;
					ostr << execMe.result_get()->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()) << std::endl;
					YaspWriteW(ostr.str().c_str());
				}
			}

			if( !checkPrompt(iMode, EXEC_MODE_MUTE) && 
                checkPrompt(iMode, EXEC_MODE_VERBOSE) && 
                bErrCatch == false)
			{
				YaspWriteW(L"\n");
			}

		}
        catch(ScilabMessage sm)
        {
            YaspWriteW(sm.GetErrorMessage().c_str());

            CallExp* pCall = dynamic_cast<CallExp*>(*j);
            if(pCall != NULL)
            {//to print call expression only of it is a macro
                ExecVisitor execFunc;
                pCall->name_get().accept(execFunc);

                if(execFunc.result_get() != NULL &&
                    (execFunc.result_get()->isMacro() || execFunc.result_get()->isMacroFile()))
                {
                    wostringstream os;

                    //add function failed
                    PrintVisitor printMe(os);
                    pCall->accept(printMe);
                    os << std::endl;

                    //add info on file failed
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("at line % 5d of exec file called by :\n"), (*j)->location_get().first_line);
                    os << szError;

                    if(ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(execFunc.result_get()->getAsCallable()->getName());
                    }


                    throw ScilabMessage(os.str(), 0, (*j)->location_get());
                }
            }

            throw ScilabMessage((*j)->location_get());
        }
		catch(ScilabError se)
		{
			//print last line
			if(!checkPrompt(iMode, EXEC_MODE_MUTE))
			{
				//printExp(&file, *j, stPrompt, &iCurrentLine, str, iMode);
			}

            if(bErrCatch)
            {
                //set mode silent for errors
                ConfigVariable::setPromptMode(ConfigVariable::silent);
            }

            //store message
            iErr = ConfigVariable::getLastErrorNumber();
            if(bErrCatch == false)
            {
                file.close();
			    return Function::Error;
            }
            break;
		}
	}

    //restore previous prompt mode
    ConfigVariable::setPromptMode(oldVal);

    if(bErrCatch)
    {
        out.push_back(new Double(iErr));
        //to lock last error information
        ConfigVariable::setLastErrorCall();
    }

	parser.freeTree();
	file.close();
	return Function::OK;
}

bool checkPrompt(int _iMode, int _iCheck)
{
	return ((_iMode & _iCheck) == _iCheck);
}

void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer, int _iMode)
{
	char strLastLine[1024];
	//case 1, exp is on 1 line and take the entire line

	//case 2, exp is multiline

	//case 3, exp is part of a line.
	//ex : 3 exp on the same line a = 1; b = 2; c = 3;

	//case 4, exp is multiline but start and/or finish in the middle of a line
	//ex :
	//a = 10;for i = 1 : a
	//	a
	//end, b = 1;

	Location loc = _pExp->location_get();

	//positionning file curser at loc.first_line
	{
		//strange case, current position is after the wanted position
		if(*_piLine > loc.first_line)
		{
			//reset line counter and restart reading at the start of the file.
			*_piLine = -1;
			_pFile->seekg( 0, ios_base::beg );
		}

		//bypass previous lines
		for(int i = *_piLine ; i < loc.first_line - 1; i++)
		{
			(*_piLine)++;
			_pFile->getline(_pstPreviousBuffer, 1024);
		}
	}

	if(loc.first_line == loc.last_line)
	{//1 line
		strncpy(strLastLine, _pstPreviousBuffer + (loc.first_column - 1), loc.last_column - (loc.first_column - 1));
		strLastLine[loc.last_column - (loc.first_column - 1)] = 0;
		printLine(_pstPrompt, strLastLine);
	}
	else
	{//multiline
		printLine(_pstPrompt, _pstPreviousBuffer + (loc.first_column - 1));

		//print other full lines
		for(int i = loc.first_line; i < (loc.last_line - 1) ; i++)
		{
			(*_piLine)++;
			_pFile->getline(_pstPreviousBuffer, 1024);
			printLine(_pstPrompt, _pstPreviousBuffer);
		}

		//last line
		_pFile->getline(_pstPreviousBuffer, 1024);
		(*_piLine)++;

		strncpy(strLastLine, _pstPreviousBuffer, loc.last_column);
		strLastLine[loc.last_column] = 0;
		printLine(_pstPrompt, strLastLine);
	}
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
