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
#include "scilabWrite.hxx"
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
#include "mopen.h"
#include "mclose.h"
#include "fullpath.h"
#include "PATH_MAX.h"
}


using namespace types;
using namespace ast;
using namespace std;

bool checkPrompt(int _iMode, int _iCheck);
void printLine(char* _stPrompt, char* _stLine, bool _bLF);
void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer);
std::string getExpression(char* _pstFile, Exp* _pExp);

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int promptMode      = 0;//default value at startup, overthise 3
    bool bPromptMode    = false;
    int iErr            = 0;
    bool bErrCatch	    = false;
    Exp* pExp		    = NULL;
    int iID             = 0;
    Parser parser;

    if(ConfigVariable::getStartFinished())
    {
        promptMode = 3;
    }

    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d to %d expected.\n"), L"exec" , 1, 3);
        return Function::Error;
    }

    if(in.size() > 1)
    {//errcatch or mode
        if(in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {//errcatch
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

            if(in.size() > 2)
            {
                if(in[2]->isDouble() == false || in[2]->getAs<Double>()->isScalar() == false)
                {//mode
                    ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"exec", 3);
                    return Function::Error;
                }

                promptMode = (int)in[2]->getAs<Double>()->getReal()[0];
                bPromptMode = true;
            }
        }
        else if(in[1]->isDouble() && in[1]->getAs<Double>()->isScalar())
        {//mode
            promptMode = (int)in[1]->getAs<Double>()->getReal()[0];
            bPromptMode = true;
        }
        else
        {//not managed
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer or string expected.\n"), L"exec", 2);
            return Function::Error;
        }
    }

    if(in[0]->isString() && in[0]->getAs<types::String>()->isScalar())
    {//1st argument is a path, parse file and execute it
        int iParsePathLen		= 0;
        String* pS = in[0]->getAs<types::String>();

        wchar_t* pstFile = pS->get(0);
        wchar_t *expandedPath = expandPathVariableW(pstFile);
        wchar_t* pwstTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        get_full_pathW(pwstTemp, (const wchar_t*)expandedPath, PATH_MAX * 2);

        /*fake call to mopen to show file within file()*/
        if(mopen(pwstTemp, L"r", 0, &iID) != MOPEN_NO_ERROR)
        {
            FREE(pwstTemp);
            ScierrorW(999, _W("%ls: Cannot open file %ls.\n"), L"exec", expandedPath);
            return Function::Error;
        }

        parser.parseFile(pwstTemp, L"exec");
        FREE(expandedPath);
        FREE(pwstTemp);
        if(parser.getExitStatus() !=  Parser::Succeded)
        {
            scilabWriteW(parser.getErrorMessage());
            delete parser.getTree();
            mclose(iID);
            return Function::Error;
        }

        pExp = parser.getTree();
    }
    else if(in[0]->isMacro())
    {//1st argument is a macro name, execute it in the current environnement
        pExp = in[0]->getAs<Macro>()->getBody();
    }
    else if(in[0]->isMacroFile())
    {//1st argument is a macro name, parse and execute it in the current environnement
        if(in[0]->getAs<MacroFile>()->parse() == false)
        {
            ScierrorW(999, _W("%ls: Unable to parse macro '%s'"), "exec", in[0]->getAs<MacroFile>()->getName().c_str());
            mclose(iID);
            return Function::Error;
        }
        pExp = in[0]->getAs<MacroFile>()->getMacro()->getBody();
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"exec", 1);
        mclose(iID);
        return Function::Error;
    }

    std::list<Exp *>::iterator j;
    std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

    char stPrompt[64];
    //get prompt
    GetCurrentPrompt(stPrompt);
    //    MessageBoxA(NULL, stPrompt, "", 0);

    wchar_t* pwstFile =  expandPathVariableW(in[0]->getAs<types::String>()->get(0));
    char* pstFile = wide_string_to_UTF8(pwstFile);
    std::ifstream file(pstFile);

    char str[1024];
    int iCurrentLine = -1; //no data in str

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();

    ConfigVariable::setPromptMode(promptMode);

    for(j = LExp.begin() ; j != LExp.end() ; j++)
    {
        try
        {
            std::list<Exp *>::iterator k = j;
            //mode == 0, print new variable but not command
            if(ConfigVariable::getPromptMode() != 0 && ConfigVariable::getPromptMode() != 2)
            {
                int iLastLine = (*j)->location_get().last_line;
                do
                {
                    printExp(&file, *k, stPrompt, &iCurrentLine, str);
                    iLastLine = (*k)->location_get().last_line;
                    k++;
                }while(k != LExp.end() && (*k)->location_get().first_line == iLastLine);
            }
            else
            {
                k++;
            }


            std::list<Exp *>::iterator p = j;
            for(; p != k; p++)
            {
                j = p;
                //excecute script
                //force -1 to prevent recursive call to exec to write in console
                //ConfigVariable::setPromptMode(-1);
                ExecVisitor execMe;
                (*j)->accept(execMe);
                //ConfigVariable::setPromptMode(promptMode);


                //to manage call without ()
                if(execMe.result_get() != NULL && execMe.result_get()->isCallable())
                {
                    Callable *pCall = execMe.result_get()->getAs<Callable>();
                    types::typed_list out;
                    types::typed_list in;
                    types::optional_list opt;

                    try
                    {
                        ExecVisitor execCall;
                        Function::ReturnValue Ret = pCall->call(in, opt, 1, out, &execCall);

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
                            os_swprintf(szError, bsiz, _W("at line % 5d of exec file called by :\n"), (*j)->location_get().first_line);
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
            }
        }
        catch(ScilabMessage sm)
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


                    mclose(iID);
                    //restore previous prompt mode
                    ConfigVariable::setPromptMode(oldVal);
                    throw ScilabMessage(os.str(), 0, (*j)->location_get());
                }
            }

            mclose(iID);
            throw ScilabMessage((*j)->location_get());
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
                file.close();
                //print failed command
                scilabError(getExpression(pstFile, *j).c_str());
                scilabErrorW(L"\n");

                //write error
                scilabErrorW(se.GetErrorMessage().c_str());

                //write position
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("at line % 5d of exec file called by :\n"), (*j)->location_get().first_line);
                scilabErrorW(szError);
                mclose(iID);
                //restore previous prompt mode
                ConfigVariable::setPromptMode(oldVal);
                //throw ScilabMessage(szError, 1, (*j)->location_get());
                //print already done, so just foward exception but with message
                throw ScilabError();
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
    mclose(iID);
    file.close();
    FREE(pstFile);
    FREE(pwstFile);
    return Function::OK;
}

std::string getExpression(char* _pstFile, Exp* _pExp)
{
    std::string out;
    char strLastLine[1024];
    char pstBuffer[1024];
    int iLine = 0;
    Location loc = _pExp->location_get();
    std::ifstream file(_pstFile);

    //bypass previous lines
    for(int i = 0 ; i < loc.first_line; i++)
    {
        file.getline(pstBuffer, 1024);
    }

    if(loc.first_line == loc.last_line)
    {
        int iStart = loc.first_column - 1;
        int iEnd = loc.last_column - 1;
        int iLen = iEnd - iStart;
        strncpy(strLastLine, pstBuffer + iStart, iLen);
        strLastLine[iLen] = 0;
        out += strLastLine;
    }
    else
    {//

        //first line, entire or not
        strcpy(strLastLine, pstBuffer + loc.first_column - 1);
        out += strLastLine;
        out += "\n";

        //print other full lines
        for(int i = loc.first_line; i < (loc.last_line - 1) ; i++)
        {
            file.getline(pstBuffer, 1024);
            out += pstBuffer;
            out += "\n";
        }


        //last line, entire or not
        file.getline(pstBuffer, 1024);
        strncpy(strLastLine, pstBuffer, loc.last_column - 1);
        strLastLine[loc.last_column - 1] = 0;
        out += strLastLine;
        out += "\n";
    }
    return out;
}

bool checkPrompt(int _iMode, int _iCheck)
{
    return ((_iMode & _iCheck) == _iCheck);
}

void printExp(std::ifstream* _pFile, Exp* _pExp, char* _pstPrompt, int* _piLine /* in/out */, char* _pstPreviousBuffer)
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
            if((*_piLine) != (loc.first_line - 1))
            {//empty line but not sequential lines
                printLine("", "", true);
            }
            _pFile->getline(_pstPreviousBuffer, 1024);
        }
    }

    if(loc.first_line == loc.last_line)
    {//1 line
        int iStart = loc.first_column - 1;
        int iEnd = loc.last_column - 1;
        int iLen = iEnd - iStart;
        strncpy(strLastLine, _pstPreviousBuffer + iStart, iLen);
        strLastLine[iLen] = 0;
        int iExpLen = (int)strlen(strLastLine);
        int iLineLen = (int)strlen(_pstPreviousBuffer);
        //printLine(_pstPrompt, strLastLine, true, false);


        if(iStart == 0 && iExpLen == iLineLen)
        {//entire line
            printLine(_pstPrompt, strLastLine, true);
        }
        else
        {
            if(iStart == 0)
            {//begin of line
                printLine(_pstPrompt, strLastLine, false);
            }
            else
            {
                if(iEnd == iLineLen)
                {
                    printLine("", strLastLine, true);
                }
                else
                {
                    printLine("", strLastLine, false);
                }
            }
        }
    }
    else
    {//multiline

        if(loc.first_column == 1)
        {
            printLine(_pstPrompt, _pstPreviousBuffer + (loc.first_column - 1), true);
        }
        else
        {
            printLine("", _pstPreviousBuffer + (loc.first_column - 1), true);
        }


        //print other full lines
        for(int i = loc.first_line; i < (loc.last_line - 1) ; i++)
        {
            (*_piLine)++;
            _pFile->getline(_pstPreviousBuffer, 1024);
            printLine(_pstPrompt, _pstPreviousBuffer, true);
        }

        //last line
        _pFile->getline(_pstPreviousBuffer, 1024);
        (*_piLine)++;

        strncpy(strLastLine, _pstPreviousBuffer, loc.last_column - 1);
        strLastLine[loc.last_column - 1] = 0;
        int iLineLen = (int)strlen(_pstPreviousBuffer);
        if(iLineLen == (loc.last_column-1))
        {
            printLine(_pstPrompt, strLastLine, true);
        }
        else
        {
            printLine(_pstPrompt, strLastLine, false);
        }
    }
}

void printLine(char* _stPrompt, char* _stLine, bool _bLF)
{
    char* sz = (char*)MALLOC(sizeof(char) * (strlen(_stPrompt) + strlen(_stLine) + 2));
    memset(sz, 0x00, sizeof(char) * (strlen(_stPrompt) + strlen(_stLine) + 2));

    if(strlen(_stPrompt) != 0)
    {
        strcat(sz, _stPrompt);
    }

    strcat(sz, _stLine);
    if(_bLF)
    {
        strcat(sz, "\n");
    }

    scilabWrite(sz);
    FREE(sz);
}
/*--------------------------------------------------------------------------*/
