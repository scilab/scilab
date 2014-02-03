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

bool checkPrompt(int _iMode, int _iCheck);
void printLine(const std::string& _stPrompt, const std::string& _stLine, bool _bLF);
std::string printExp(std::ifstream& _File, Exp* _pExp, const std::string& _stPrompt, int* _piLine /* in/out */, int* _piCol /* in/out */, std::string& _stPreviousBuffer);
std::string getExpression(const std::string& _stFile, Exp* _pExp);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int promptMode      = 0;//default value at startup, overthise 3 or verbose ";"
    bool bPromptMode    = false;
    int iErr            = 0;
    bool bErrCatch      = false;
    Exp* pExp           = NULL;
    int iID             = 0;
    Parser parser;

    if (ConfigVariable::getStartFinished())
    {
        if (ConfigVariable::getVerbose())
        {
            promptMode = 3;
        }
        else
        {
            promptMode = -1;
        }
    }

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "exec" , 1, 3);
        return Function::Error;
    }

    if (in.size() > 1)
    {
        //errcatch or mode
        if (in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {
            //errcatch
            String* pS = in[1]->getAs<types::String>();
            if (os_wcsicmp(pS->get(0), L"errcatch") == 0)
            {
                bErrCatch = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), "exec", 2);
                return Function::Error;
            }

            if (in.size() > 2)
            {
                if (in[2]->isDouble() == false || in[2]->getAs<Double>()->isScalar() == false)
                {
                    //mode
                    Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "exec", 3);
                    return Function::Error;
                }

                promptMode = (int)in[2]->getAs<Double>()->getReal()[0];
                bPromptMode = true;
            }
        }
        else if (in[1]->isDouble() && in[1]->getAs<Double>()->isScalar())
        {
            //mode
            promptMode = (int)in[1]->getAs<Double>()->getReal()[0];
            bPromptMode = true;
        }
        else
        {
            //not managed
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer or string expected.\n"), "exec", 2);
            return Function::Error;
        }
    }

    if (in[0]->isString() && in[0]->getAs<types::String>()->isScalar())
    {
        //1st argument is a path, parse file and execute it
        int iParsePathLen		= 0;
        String* pS = in[0]->getAs<types::String>();

        wchar_t* pstFile = pS->get(0);
        wchar_t *expandedPath = expandPathVariableW(pstFile);
        wchar_t* pwstTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        get_full_pathW(pwstTemp, (const wchar_t*)expandedPath, PATH_MAX * 2);

        /*fake call to mopen to show file within file()*/
        if (mopen(pwstTemp, L"r", 0, &iID) != MOPEN_NO_ERROR)
        {
            FREE(pwstTemp);
            char* pstPath = wide_string_to_UTF8(expandedPath);
            Scierror(999, _("%s: Cannot open file %s.\n"), "exec", pstPath);
            FREE(pstPath);
            return Function::Error;
        }

        parser.parseFile(pwstTemp, L"exec");
        FREE(expandedPath);
        FREE(pwstTemp);
        if (parser.getExitStatus() !=  Parser::Succeded)
        {
            scilabWriteW(parser.getErrorMessage());
            delete parser.getTree();
            mclose(iID);
            return Function::Error;
        }

        pExp = parser.getTree();
    }
    else if (in[0]->isMacro())
    {
        //1st argument is a macro name, execute it in the current environnement
        pExp = in[0]->getAs<Macro>()->getBody();
    }
    else if (in[0]->isMacroFile())
    {
        //1st argument is a macro name, parse and execute it in the current environnement
        if (in[0]->getAs<MacroFile>()->parse() == false)
        {
            char* pstMacro = wide_string_to_UTF8(in[0]->getAs<MacroFile>()->getName().c_str());
            Scierror(999, _("%s: Unable to parse macro '%s'"), "exec", pstMacro);
            FREE(pstMacro);
            mclose(iID);
            return Function::Error;
        }
        pExp = in[0]->getAs<MacroFile>()->getMacro()->getBody();
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "exec", 1);
        return Function::Error;
    }

    std::list<Exp *>::iterator j;
    std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

    char pstPrompt[64];
    //get prompt
    GetCurrentPrompt(pstPrompt);
    std::string stPrompt(pstPrompt);
    //    MessageBoxA(NULL, stPrompt, "", 0);

    wchar_t* pwstFile =  expandPathVariableW(in[0]->getAs<types::String>()->get(0));
    char* pstFile = wide_string_to_UTF8(pwstFile);
    std::string stFile(pstFile);
    std::ifstream file(pstFile);

    std::string str;
    int iCurrentLine = -1; //no data in str
    int iCurrentCol = 0; //no data in str

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();

    ConfigVariable::setPromptMode(promptMode);

    for (j = LExp.begin() ; j != LExp.end() ; j++)
    {
        try
        {
            std::list<Exp *>::iterator k = j;
            //mode == 0, print new variable but not command
            if (ConfigVariable::getPromptMode() != 0 && ConfigVariable::getPromptMode() != 2)
            {
                int iLastLine = (*j)->location_get().last_line;
                do
                {
                    str = printExp(file, *k, stPrompt, &iCurrentLine, &iCurrentCol, str);
                    iLastLine = (*k)->location_get().last_line;
                    k++;
                }
                while (k != LExp.end() && (*k)->location_get().first_line == iLastLine);

                // In case where the line ends by spaces, iCurrentCol is not reset
                // by printExp because we don't know if that's the end of the expression
                // before go out of the loop. So we have to reset column count
                // and print a new line before manage the next line.
                if (iCurrentCol != 0)
                {
                    iCurrentCol = 0;
                    printLine("", "", true);
                }
            }
            else
            {
                k++;
            }


            std::list<Exp *>::iterator p = j;
            for (; p != k; p++)
            {
                bool bImplicitCall = false;
                j = p;
                //excecute script
                //force -1 to prevent recursive call to exec to write in console
                //ConfigVariable::setPromptMode(-1);
                ExecVisitor execMe;
                (*j)->accept(execMe);
                //ConfigVariable::setPromptMode(promptMode);


                //to manage call without ()
                if (execMe.result_get() != NULL && execMe.result_get()->isCallable())
                {
                    Callable *pCall = execMe.result_get()->getAs<Callable>();
                    types::typed_list out;
                    types::typed_list in;
                    types::optional_list opt;

                    try
                    {
                        //in this case of calling, we can return only one values
                        ExecVisitor execCall;
                        execCall.expected_setSize(1);
                        Function::ReturnValue Ret = pCall->call(in, opt, 1, out, &execCall);

                        if (Ret == Callable::OK)
                        {
                            if (out.size() == 0)
                            {
                                execMe.result_set(NULL);
                            }
                            else if (out.size() == 1)
                            {
                                out[0]->DecreaseRef();
                                execMe.result_set(out[0]);
                                bImplicitCall = true;
                            }
                            else
                            {
                                for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                                {
                                    out[i]->DecreaseRef();
                                    execMe.result_set(i, out[i]);
                                }
                            }
                        }
                        else if (Ret == Callable::Error)
                        {
                            if (ConfigVariable::getLastErrorFunction() == L"")
                            {
                                ConfigVariable::setLastErrorFunction(pCall->getName());
                            }

                            if (pCall->isMacro() || pCall->isMacroFile())
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
                    catch (ScilabMessage sm)
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        (*j)->accept(printMe);
                        os << std::endl << std::endl;
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                        }

                        if (pCall->isMacro() || pCall->isMacroFile())
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

                //update ans variable
                SimpleVar* pVar = dynamic_cast<SimpleVar*>(*j);
                //don't output Simplevar and empty result
                if (execMe.result_get() != NULL && (pVar == NULL || bImplicitCall))
                {
                    InternalType* pITAns = execMe.result_get();
                    symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *pITAns);
                    if ( (*j)->is_verbose() && bErrCatch == false)
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        execMe.VariableToString(pITAns, L"ans");
                    }
                }
            }
        }
        catch (ScilabMessage sm)
        {
            scilabErrorW(sm.GetErrorMessage().c_str());

            CallExp* pCall = dynamic_cast<CallExp*>(*j);
            if (pCall != NULL)
            {
                //to print call expression only of it is a macro
                ExecVisitor execFunc;
                pCall->name_get().accept(execFunc);

                if (execFunc.result_get() != NULL &&
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

                    if (ConfigVariable::getLastErrorFunction() == L"")
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
        catch (ScilabError se)
        {
            if (ConfigVariable::getLastErrorNumber() == 0)
            {
                ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                ConfigVariable::setLastErrorFunction(wstring(L""));
            }

            //store message
            iErr = ConfigVariable::getLastErrorNumber();
            if (bErrCatch == false)
            {
                file.close();
                //print failed command
                scilabError(getExpression(stFile, *j).c_str());
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

    if (bErrCatch)
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

std::string getExpression(const std::string& _stFile, Exp* _pExp)
{
    std::string out;
    std::string stBuffer;
    int iLine = 0;
    Location loc = _pExp->location_get();
    std::ifstream file(_stFile.c_str());

    //bypass previous lines
    for (int i = 0 ; i < loc.first_line; i++)
    {
        std::getline(file, stBuffer);
    }

    if (loc.first_line == loc.last_line)
    {
        int iStart = loc.first_column - 1;
        int iEnd = loc.last_column - 1;
        int iLen = iEnd - iStart;
        out += string(stBuffer.c_str() + iStart, iLen);
    }
    else
    {
        //

        //first line, entire or not
        out += string(stBuffer.c_str() + loc.first_column - 1);
        out += "\n";

        //print other full lines
        for (int i = loc.first_line; i < (loc.last_line - 1) ; i++)
        {
            std::getline(file, stBuffer);
            out += stBuffer;
            out += "\n";
        }


        //last line, entire or not
        getline(file, stBuffer);
        out += string(stBuffer.c_str(), loc.last_column - 1);
        out += "\n";
    }
    return out;
}

bool checkPrompt(int _iMode, int _iCheck)
{
    return ((_iMode & _iCheck) == _iCheck);
}

std::string printExp(std::ifstream& _File, Exp* _pExp, const std::string& _stPrompt, int* _piLine /* in/out */, int* _piCol /* in/out */, std::string& _stPreviousBuffer)
{
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
        if (*_piLine > loc.first_line)
        {
            //reset line counter and restart reading at the start of the file.
            *_piLine = -1;
            _File.seekg( 0, ios_base::beg );
        }

        //bypass previous lines
        for (int i = *_piLine ; i < loc.first_line - 1; i++)
        {

            (*_piLine)++;
            if ((*_piLine) != (loc.first_line - 1))
            {
                //empty line but not sequential lines
                printLine("", "", true);
            }
            std::getline(_File, _stPreviousBuffer);
        }
    }

    if (loc.first_line == loc.last_line)
    {
        //1 line
        int iStart = loc.first_column - 1;
        int iEnd = loc.last_column - 1;
        int iLen = iEnd - iStart;
        std::string strLastLine(_stPreviousBuffer.c_str() + iStart, iLen);
        int iExpLen = iLen;
        int iLineLen = (int)_stPreviousBuffer.size();
        //printLine(_pstPrompt, strLastLine, true, false);

        if (iStart == 0 && iExpLen == iLineLen)
        {
            //entire line
            if (*_piCol)
            {
                //blank char at the end of previous line
                printLine("", "", true);
            }
            printLine(_stPrompt, strLastLine, true);
            *_piCol = 0;
        }
        else
        {
            if (iStart == 0)
            {
                //begin of line
                if (*_piCol)
                {
                    //blank char at the end of previous line
                    printLine("", "", true);
                }
                printLine(_stPrompt, strLastLine, false);
                *_piCol = loc.last_column;
            }
            else
            {
                if (*_piCol == 0)
                {
                    printLine(_stPrompt, "", false);
                    (*_piCol)++;
                }

                if (*_piCol < loc.first_column)
                {
                    //pickup separator between expressionsfrom file and add to output
                    int iSize = loc.first_column - *_piCol;
                    std::string stTemp(_stPreviousBuffer.c_str() +  (*_piCol - 1), iSize);
                    printLine("", stTemp, false);
                    *_piCol = loc.first_column;
                }

                if (iEnd == iLineLen)
                {
                    printLine("", strLastLine, true);
                    *_piCol = 0;
                }
                else
                {
                    printLine("", strLastLine, false);
                    *_piCol = loc.last_column;
                }
            }
        }
    }
    else
    {
        //multiline

        if (loc.first_column == 1)
        {
            if (*_piCol)
            {
                //blank char at the end of previous line
                printLine("", "", true);
            }
            printLine(_stPrompt, _stPreviousBuffer.c_str() + (loc.first_column - 1), false);
        }
        else
        {
            if (*_piCol < loc.first_column)
            {
                //pickup separator between expressionsfrom file and add to output
                int iSize = loc.first_column - *_piCol;
                std::string stTemp(_stPreviousBuffer.c_str() +  (*_piCol - 1), iSize);
                printLine("", stTemp, false);
                *_piCol = loc.first_column;
            }

            printLine("", _stPreviousBuffer.c_str() + (loc.first_column - 1), false);
        }

        //print other full lines
        for (int i = loc.first_line; i < (loc.last_line - 1) ; i++)
        {
            (*_piLine)++;
            std::getline(_File, _stPreviousBuffer);
            printLine(_stPrompt, _stPreviousBuffer.c_str(), false);
        }

        //last line
        std::getline(_File, _stPreviousBuffer);
        (*_piLine)++;

        int iSize = loc.last_column - 1;
        std::string stLastLine(_stPreviousBuffer.c_str(), iSize);
        int iLineLen = (int)_stPreviousBuffer.size();
        if (iLineLen == iSize)
        {
            printLine(_stPrompt, stLastLine, true);
            *_piCol = 0;
        }
        else
        {
            printLine(_stPrompt, stLastLine, false);
            *_piCol = loc.last_column;
        }
    }

    return _stPreviousBuffer;
}

void printLine(const std::string& _stPrompt, const std::string& _stLine, bool _bLF)
{
    std::string st;

    if (_stPrompt.size() != 0)
    {
        st = "\n" + _stPrompt;
    }

    st += _stLine;
    if (_bLF)
    {
        st += "\n";
    }

    scilabWrite(st.c_str());
}
/*--------------------------------------------------------------------------*/
