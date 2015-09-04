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
#include "functions_gw.hxx"

#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "types_tools.hxx"
#include "runner.hxx"
#include "threadmanagement.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "filemanager.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "os_string.h"
#include "expandPathVariable.h"
#include "prompt.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#include "mopen.h"
#include "mclose.h"
#include "fullpath.h"
#include "PATH_MAX.h"
}

using namespace ast;
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
    types::Macro* pMacro = NULL;
    Parser parser;

    wchar_t* pwstFile = NULL;
    char* pstFile = NULL;

    std::string stFile;
    std::ifstream* file = NULL;
    std::wstring wstFile;

    if (ConfigVariable::getStartProcessing() == false)
    {
        if (ConfigVariable::getVerbose())
        {
            promptMode = 3;
        }
        else
        {
            promptMode = 0;
        }
    }

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "exec" , 1, 3);
        return Function::Error;
    }

    // get mode and errcatch
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
            if (in.size() > 2)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), "exec", 2);
                return Function::Error;
            }
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
        int iParsePathLen = 0;
        String* pS = in[0]->getAs<types::String>();

        pwstFile = expandPathVariableW(pS->get(0));
        pstFile = wide_string_to_UTF8(pwstFile);
        stFile = pstFile;
        file = new std::ifstream(pstFile);

        FREE(pstFile);

        wchar_t* pwstTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
        get_full_pathW(pwstTemp, pwstFile, PATH_MAX * 2);
        wstFile = pwstTemp;

        FREE(pwstFile);
        /*fake call to mopen to show file within file()*/
        if (mopen(pwstTemp, L"r", 0, &iID) != MOPEN_NO_ERROR)
        {
            file->close();
            delete file;
            FREE(pwstTemp);
            Scierror(999, _("%s: Cannot open file %s.\n"), "exec", stFile.data());
            return Function::Error;
        }

        ThreadManagement::LockParser();
        parser.parseFile(pwstTemp, L"exec");
        FREE(pwstTemp);
        if (parser.getExitStatus() !=  Parser::Succeded)
        {
            file->close();
            delete file;
            if (bErrCatch)
            {
                out.push_back(new Double(999));
                //to lock last error information
                ConfigVariable::setLastErrorCall();
                ConfigVariable::setLastErrorMessage(parser.getErrorMessage());
                ConfigVariable::setLastErrorNumber(999);
                delete parser.getTree();
                // Check if file has not already been closed (for ex mclose('all') in function)
                if (FileManager::isOpened(wstFile.data()) == true)
                {
                    mclose(iID);
                }
                ThreadManagement::UnlockParser();
                return Function::OK;
            }

            char* pst = wide_string_to_UTF8(parser.getErrorMessage());
            Scierror(999, "%s", pst);
            FREE(pst);

            delete parser.getTree();
            // Check if file has not already been closed (for ex mclose('all') in function)
            if (FileManager::isOpened(wstFile.data()) == true)
            {
                mclose(iID);
            }
            ThreadManagement::UnlockParser();
            return Function::Error;
        }

        if (ConfigVariable::getSerialize())
        {
            ast::Exp* temp = parser.getTree();
            if (ConfigVariable::getTimed())
            {
                pExp = callTyper(temp, L"exec");
            }
            else
            {
                pExp = callTyper(temp);
            }

            delete temp;
        }
        else
        {
            pExp = parser.getTree();
        }

        ThreadManagement::UnlockParser();
        // update where to set the name of the executed file.
        ConfigVariable::setFileNameToLastWhere(wstFile.data());

        ConfigVariable::setExecutedFileID(iID);
    }
    else if (in[0]->isMacro() || in[0]->isMacroFile())
    {
        typed_list input;
        optional_list optional;
        typed_list output;
        ast::ExecVisitor execFunc;

        if (in[0]->isMacroFile())
        {
            //1st argument is a macro name, parse and execute it in the current environnement
            if (in[0]->getAs<MacroFile>()->parse() == false)
            {
                char* pstMacro = wide_string_to_UTF8(in[0]->getAs<MacroFile>()->getName().c_str());
                Scierror(999, _("%s: Unable to parse macro '%s'"), "exec", pstMacro);
                FREE(pstMacro);
                return Function::Error;
            }
            pMacro = in[0]->getAs<MacroFile>()->getMacro();
        }
        else //1st argument is a macro name, execute it in the current environnement
        {
            pMacro = in[0]->getAs<Macro>();
        }

        // unable for macro with varargin or varargout
        auto inputs = pMacro->getInputs();
        auto outputs = pMacro->getOutputs();
        if ((inputs->size() != 0 && inputs->back()->getSymbol().getName() == L"varargin") ||
                outputs->size() != 0 && outputs->back()->getSymbol().getName() == L"varargout")
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A macro without varargin and varargout expected.\n"), "exec", 1);
            return Function::Error;
        }

        pExp = pMacro->getBody();

        // update where to set the name of the executed macro instead of "exec"
        ConfigVariable::WhereEntry lastWhere = ConfigVariable::getWhere().back();
        int iLine = lastWhere.m_line;
        int iAbsLine = lastWhere.m_absolute_line;
        ConfigVariable::where_end();
        ConfigVariable::where_begin(iLine, iAbsLine, pMacro);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "exec", 1);
        return Function::Error;
    }

    if (pMacro)
    {
        //store the line number where is stored this macro in file.
        ConfigVariable::macroFirstLine_begin(pMacro->getFirstLine());
    }

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(promptMode);

    // if not exp displaying, just execute the seqexp
    if (file == NULL || promptMode == 0 || promptMode == 2)
    {
        ast::SeqExp* pSeqExp = pExp->getAs<SeqExp>();

        try
        {
            ExecVisitor execExps;
            pSeqExp->accept(execExps);
        }
        catch (const ast::InternalAbort& ia)
        {
            if (file)
            {
                file->close();
                delete file;
                delete pExp;
            }

            throw ia;
        }
        catch (const ast::InternalError& ie)
        {
            if (bErrCatch == false)
            {
                if (file)
                {
                    file->close();
                    delete file;
                    delete pExp;
                }

                ConfigVariable::setPromptMode(oldVal);
                ConfigVariable::setExecutedFileID(0);
                throw ie;
            }

            ConfigVariable::resetWhereError();
            iErr = ConfigVariable::getLastErrorNumber();
        }
    }
    else
    {
        ast::exps_t& LExp = pExp->getAs<SeqExp>()->getExps();

        char pstPrompt[64];
        //get prompt
        GetCurrentPrompt(pstPrompt);
        std::string stPrompt(pstPrompt);

        std::string str;
        int iCurrentLine = -1; //no data in str
        int iCurrentCol = 0; //no data in str

        for (ast::exps_t::iterator j = LExp.begin(), itEnd = LExp.end() ; j != itEnd; ++j)
        {
            // printf some exp
            ast::exps_t::iterator k = j;
            int iLastLine = (*j)->getLocation().last_line;
            do
            {
                str = printExp(*file, *k, stPrompt, &iCurrentLine, &iCurrentCol, str);
                iLastLine = (*k)->getLocation().last_line;
                k++;
            }
            while (k != LExp.end() && (*k)->getLocation().first_line == iLastLine);

            // In case where the line ends by spaces, iCurrentCol is not reset
            // by printExp because we don't know if that's the end of the expression
            // before go out of the loop. So we have to reset column count
            // and print a new line before manage the next line.
            if (iCurrentCol != 0)
            {
                iCurrentCol = 0;
                printLine("", "", true);
            }

            // create a seqexp with printed exp
            ast::exps_t* someExps = new ast::exps_t();
            someExps->assign(j, k);
            k--;
            SeqExp seqExp(Location((*j)->getLocation().first_line,      (*k)->getLocation().last_line,
                                   (*j)->getLocation().first_column,    (*k)->getLocation().last_column),
                          *someExps);

            j = k;

            try
            {
                // execute printed exp
                ExecVisitor execExps;
                seqExp.accept(execExps);
            }
            catch (const ast::InternalAbort& ia)
            {
                if (file)
                {
                    delete pExp;
                    // Check if file has not already been closed (for ex mclose('all') in function)
                    if (FileManager::isOpened(wstFile.data()) == true)
                    {
                        mclose(iID);
                    }
                    file->close();
                    delete file;
                }

                //restore previous prompt mode
                ConfigVariable::setPromptMode(oldVal);

                // avoid double delete on exps when "seqExp" is destryed and "LExp" too
                ast::exps_t& protectExp = seqExp.getExps();
                for (int i = 0; i < protectExp.size(); ++i)
                {
                    protectExp[i] = NULL;
                }

                throw ia;
            }
            catch (const ast::InternalError& ie)
            {
                ConfigVariable::setExecutedFileID(0);
                ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);
                if (ConfigVariable::getLastErrorLine() == 0)
                {
                    ConfigVariable::setLastErrorLine(ie.GetErrorLocation().first_line);
                }

                //store message
                iErr = ConfigVariable::getLastErrorNumber();
                if (bErrCatch == false)
                {
                    if (file)
                    {
                        delete pExp;
                        // Check if file has not already been closed (for ex mclose('all') in function)
                        if (FileManager::isOpened(wstFile.data()) == true)
                        {
                            mclose(iID);
                        }
                        file->close();
                        delete file;
                    }

                    //restore previous prompt mode
                    ConfigVariable::setPromptMode(oldVal);

                    // avoid double delete on exps when "seqExp" is destryed and "LExp" too
                    ast::exps_t& protectExp = seqExp.getExps();
                    for (int i = 0; i < protectExp.size(); ++i)
                    {
                        protectExp[i] = NULL;
                    }

                    throw ie;
                }

                if (pMacro)
                {
                    // reset last first line of macro called
                    ConfigVariable::macroFirstLine_end();
                }

                ConfigVariable::resetWhereError();
                break;
            }

            ConfigVariable::setExecutedFileID(0);

            // avoid double delete on exps when "seqExp" is destryed and "LExp" too
            ast::exps_t& protectExp = seqExp.getExps();
            for (int i = 0; i < protectExp.size(); ++i)
            {
                protectExp[i] = NULL;
            }
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

    if (file)
    {
        delete pExp;
        // Check if file has not already been closed (for ex mclose('all') in function)
        if (FileManager::isOpened(wstFile.data()) == true)
        {
            mclose(iID);
        }

        file->close();
        delete file;
    }

    return Function::OK;
}

std::string getExpression(const std::string& _stFile, Exp* _pExp)
{
    std::string out;
    std::string stBuffer;
    int iLine = 0;
    Location loc = _pExp->getLocation();
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

    Location loc = _pExp->getLocation();

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
                printLine(_stPrompt, _stPreviousBuffer.c_str(), false);
                *_piCol = loc.first_column;
            }
        }

        //print other full lines
        for (int i = loc.first_line; i < (loc.last_line - 1) ; i++)
        {
            (*_piLine)++;
            std::getline(_File, _stPreviousBuffer);
            // dont print empty line of function body
            if (_stPreviousBuffer.size() != 0)
            {
                printLine(_stPrompt, _stPreviousBuffer.c_str(), false);
            }
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
    if (_bLF && ConfigVariable::isEmptyLineShow())
    {
        st += "\n";
    }

    scilabWrite(st.c_str());
}
/*--------------------------------------------------------------------------*/
