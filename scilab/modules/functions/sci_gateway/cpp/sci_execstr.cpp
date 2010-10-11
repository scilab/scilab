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
#include "yaspio.hxx"
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
        if(in[1]->getType() != InternalType::RealString || in[1]->getAsString()->size_get() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"execstr", 2);
            return Function::Error;
        }

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
    }

	//3rd parameter
	if(in.size() == 3)
	{
        if(in[2]->getType() != InternalType::RealString || in[2]->getAsString()->size_get() != 1)
		{
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"execstr", 3);
            return Function::Error;
        }

		if(os_wcsicmp(in[2]->getAsString()->string_get(0), MUTE_FLAG) == 0)
        {
            bMute = true;
        }
        else if(os_wcsicmp(in[2]->getAsString()->string_get(0), NO_MUTE_FLAG) == 0)
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
    if(in[0]->isDouble() && in[0]->getAsDouble()->size_get() == 0)
    {// execstr([])
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if(in[0]->getType() != InternalType::RealString || (in[0]->getAsString()->rows_get() != 1 && in[0]->getAsString()->cols_get() != 1))
	{
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Vector of strings expected.\n"), L"execstr", 1);
        return Function::Error;
    }

	String* pS = in[0]->getAsString();
	int iTotalLen = pS->size_get(); //add \n after each string
	for(int i = 0 ; i < pS->size_get() ; i++)
	{
		iTotalLen += (int)wcslen(pS->string_get(i));
	}

	pstCommand = (wchar_t*)MALLOC(sizeof(wchar_t) * (iTotalLen + 1));//+1 for null termination

	for(int i = 0, iPos = 0 ; i < pS->size_get() ; i++)
	{
		wcscpy(pstCommand + iPos, pS->string_get(i));
		iPos = (int)wcslen(pstCommand);
		pstCommand[iPos++] = L'\n';
		pstCommand[iPos] = 0;
	}

	parser.parse(pstCommand);
    FREE(pstCommand);
	if(parser.getExitStatus() !=  Parser::Succeded)
	{
        ScierrorW(999, L"%s", parser.getErrorMessage());
		return Function::Error;
	}

	pExp = parser.getTree();

	if(pExp == NULL)
	{
		return Function::Error;
	}

    if(bMute)
    {
	    MuteVisitor mute;
	    pExp->accept(mute);
    }

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

    //save current prompt mode
    ConfigVariable::PromptMode oldVal = ConfigVariable::getPromptMode();
	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
			//excecute script
			ExecVisitor execMe;
			(*j)->accept(execMe);
		}
		catch(ScilabError se)
		{
            if(bErrCatch && bMute == true)
            {
                //set mode silent for errors
                ConfigVariable::setPromptMode(ConfigVariable::silent);
            }

            //store message
            ScierrorW(se.GetErrorNumber(), L"%ls", se.GetErrorMessage().c_str());
            iErr = se.GetErrorNumber();
            if(bErrCatch == false)
            {
            	parser.freeTree();
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
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
