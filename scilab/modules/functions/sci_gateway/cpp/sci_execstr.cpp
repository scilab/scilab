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
#include "functions_gw.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "MALLOC.h"
#include "os_wcsicmp.h"
#include "Scierror.h"
#include "localization.h"
}

#define MUTE_FLAG       L"n"
#define NO_MUTE_FLAG    L"m"

using namespace std;
using namespace types;
using namespace ast;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_execstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iErr            = 0;
	bool bErrCatch		= false;
    bool bMute          = true;
	wchar_t* pstMsg     = NULL;
	Exp* pExp           = NULL;
	wchar_t *pstCommand = NULL;
    Parser parser;

	if(in.size() < 1 || in.size() > 3)
	{
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d to %d expected.\n"), L"execstr" , 1, 3);
        return Function::Error;
	}

    //2nd parameter
	if(in.size() > 1)
	{//errcatch
        if(in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"execstr", 2);
            return Function::Error;
        }

        String* pS = in[1]->getAs<types::String>();
        if(os_wcsicmp(pS->get(0), L"errcatch") == 0)
        {
            bErrCatch = true;
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: 'errcatch' expected.\n"), L"execstr", 2);
            return Function::Error;
        }
    }

	//3rd parameter
	if(in.size() == 3)
	{
        if(in[2]->isString() == false || in[2]->getAs<types::String>()->getSize() != 1)
		{
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"execstr", 3);
            return Function::Error;
        }

		if(os_wcsicmp(in[2]->getAs<types::String>()->get(0), MUTE_FLAG) == 0)
        {
            bMute = true;
        }
        else if(os_wcsicmp(in[2]->getAs<types::String>()->get(0), NO_MUTE_FLAG) == 0)
        {
            bMute = false;
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), L"execstr", 3, MUTE_FLAG, NO_MUTE_FLAG);
            return Function::Error;
        }
	}

    //1st argument
    if(in[0]->isDouble() && in[0]->getAs<Double>()->getSize() == 0)
    {// execstr([])
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if(in[0]->isString() == false || (in[0]->getAs<types::String>()->getRows() != 1 && in[0]->getAs<types::String>()->getCols() != 1))
	{
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Vector of strings expected.\n"), L"execstr", 1);
        return Function::Error;
    }

	String* pS = in[0]->getAs<types::String>();
	int iTotalLen = pS->getSize(); //add \n after each string
	for(int i = 0 ; i < pS->getSize() ; i++)
	{
		iTotalLen += (int)wcslen(pS->get(i));
	}

	pstCommand = (wchar_t*)MALLOC(sizeof(wchar_t) * (iTotalLen + 1));//+1 for null termination

	for(int i = 0, iPos = 0 ; i < pS->getSize() ; i++)
	{
		wcscpy(pstCommand + iPos, pS->get(i));
		iPos = (int)wcslen(pstCommand);
		pstCommand[iPos++] = L'\n';
		pstCommand[iPos] = 0;
	}

	parser.parse(pstCommand);
    FREE(pstCommand);
	if(parser.getExitStatus() !=  Parser::Succeded)
	{
        ScierrorW(999, L"%ls", parser.getErrorMessage());
		return Function::Error;
	}

	pExp = parser.getTree();

	if(pExp == NULL)
	{
		return Function::Error;
	}

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    if(bMute)
    {
        ConfigVariable::setPromptMode(-1);
    }

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
            //excecute script
            ExecVisitor execMe;
            (*j)->accept(execMe);

            //to manage call without ()
            if(execMe.result_get() != NULL && execMe.result_get()->isCallable())
            {
                Callable *pCall = execMe.result_get()->getAs<Callable>();
                types::typed_list out;
                types::typed_list in;

                try
                {
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
                catch(ScilabMessage sm)
                {
                    wostringstream os;
                    PrintVisitor printMe(os);
                    (*j)->accept(printMe);
                    os << std::endl << std::endl;
                    if(ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(pCall->getName());
                    }

                    if(pCall->isMacro() || pCall->isMacroFile())
                    {
                        wstring szAllError;
                        wchar_t szError[bsiz];
                        os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                        szAllError = szError + os.str();
                        os_swprintf(szError, bsiz, _W("in  execstr instruction    called by :\n"));
                        szAllError += szError;
                        throw ScilabMessage(szAllError);
                    }
                    else
                    {
                        sm.SetErrorMessage(sm.GetErrorMessage() + os.str());
                        throw sm;
                    }
                }
            }

            //update ans variable.
			if(execMe.result_get() != NULL && execMe.result_get()->isDeletable())
			{
                InternalType* pITAns = execMe.result_get();
				symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *pITAns);
				if( (*j)->is_verbose() && bErrCatch == false)
				{
					std::wostringstream ostr;
					ostr << L" ans  =" << std::endl;
					ostr << std::endl;
                    pITAns->toString(ostr);
					ostr << std::endl;
					scilabWriteW(ostr.str().c_str());
				}
			}

			//if( !checkPrompt(iMode, EXEC_MODE_MUTE) &&
   //             bErrCatch == false)
			//{
			//	scilabWriteW(L"\n");
			//}
		}
        catch(ScilabMessage sm)
        {
            if(bErrCatch  == false && bMute == false)
            {
                scilabErrorW(sm.GetErrorMessage().c_str());

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
                            ConfigVariable::setLastErrorFunction(execFunc.result_get()->getAs<Callable>()->getName());
                        }

                        //restore previous prompt mode
                        ConfigVariable::setPromptMode(oldVal);
                        throw ScilabMessage(os.str(), 0, (*j)->location_get());
                    }
                }
                throw ScilabMessage((*j)->location_get());
            }
            else
            {
                iErr = ConfigVariable::getLastErrorNumber();
                break;
            }
        }
		catch(ScilabError se)
		{
            if(ConfigVariable::getLastErrorMessage() == L"")
            {
                ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                ConfigVariable::setLastErrorFunction(wstring(L""));
            }

            //store message
            iErr = ConfigVariable::getLastErrorNumber();
            if(bErrCatch == false)
            {
                //in case of error, change mode to 2 ( prompt )
                ConfigVariable::setPromptMode(2);
                //write error
                scilabErrorW(se.GetErrorMessage().c_str());

                //write positino
                ScierrorW(999, _W("in  execstr instruction    called by :\n"));
                //restore previous prompt mode
                ConfigVariable::setPromptMode(oldVal);
                //throw ScilabMessage(szError, 1, (*j)->location_get());
                //print already done, so just foward exception but with message
                //throw ScilabError();
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

    delete parser.getTree();
	return Function::OK;
}
/*--------------------------------------------------------------------------*/

/*
		catch(ScilabError se)
		{
            if(bErrCatch && bMute == true)
            {
                //set mode silent for errors
                ConfigVariable::setPromptMode(-1);
            }

            //store message
            ScierrorW(ConfigVariable::getLastErrorNumber(), L"%ls", ConfigVariable::getLastErrorMessage().c_str());
            iErr = ConfigVariable::getLastErrorNumber();
            if(bErrCatch == false)
            {
            	delete parser.getTree();
			    return Function::Error;
            }
            break;
		}
*/

/*
*/
