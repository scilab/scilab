/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include "functions_gw.hxx"

#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
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

#include <memory>
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

void closeFile(std::ifstream* file, int fileId, const std::wstring& wstFile, ast::Exp* pExp)
{
    if (file)
    {
        file->close();
        delete file;

        if (pExp)
        {
            delete pExp;
        }

        // Check if file has not already been closed (for ex mclose('all') in function)
        if (FileManager::isOpened(wstFile))
        {
            mclose(fileId);
        }
    }
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_exec(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int promptMode      = 0;//default value at startup, overthise 3 or verbose ";"
    int iErr            = 0;
    bool bErrCatch      = false;
    ast::Exp* pExp      = NULL;
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
        return types::Function::Error;
    }

    // get mode and errcatch
    if (in.size() > 1)
    {
        //errcatch or mode
        if (in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {
            //errcatch
            types::String* pS = in[1]->getAs<types::String>();
            if (os_wcsicmp(pS->get(0), L"errcatch") == 0)
            {
                bErrCatch = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), "exec", 2);
                return types::Function::Error;
            }

            if (in.size() > 2)
            {

                if (in[2]->isDouble() == false || in[2]->getAs<types::Double>()->isScalar() == false)
                {
                    //mode
                    Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "exec", 3);
                    return types::Function::Error;
                }

                promptMode = (int)in[2]->getAs<types::Double>()->getReal()[0];
            }
        }
        else if (in[1]->isDouble() && in[1]->getAs<types::Double>()->isScalar())
        {
            if (in.size() > 2)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), "exec", 2);
                return types::Function::Error;
            }
            //mode
            promptMode = (int)in[1]->getAs<types::Double>()->getReal()[0];
        }
        else
        {
            //not managed
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer or string expected.\n"), "exec", 2);
            return types::Function::Error;
        }
    }

    if (in[0]->isString() && in[0]->getAs<types::String>()->isScalar())
    {
        //1st argument is a path, parse file and execute it
        int iParsePathLen = 0;
        types::String* pS = in[0]->getAs<types::String>();

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
            closeFile(file, iID, wstFile, pExp);
            FREE(pwstTemp);
            Scierror(999, _("%s: Cannot open file %s.\n"), "exec", stFile.data());
            return types::Function::Error;
        }

        // update where to set the name of the executed file.
        ConfigVariable::setFileNameToLastWhere(wstFile.data());

        ThreadManagement::LockParser();
        parser.parseFile(pwstTemp, L"exec");
        FREE(pwstTemp);
        if (parser.getExitStatus() !=  Parser::Succeded)
        {
            closeFile(file, iID, wstFile, pExp);
            if (bErrCatch)
            {
                out.push_back(new types::Double(999));
                //to lock last error information
                ConfigVariable::setLastErrorCall();
                // when the parser can not open file the error is already set in lasterror.
                if (wcscmp(parser.getErrorMessage(), L""))
                {
                    ConfigVariable::setLastErrorMessage(parser.getErrorMessage());
                    ConfigVariable::setLastErrorNumber(999);
                }
                delete parser.getTree();
                ThreadManagement::UnlockParser();
                return types::Function::OK;
            }

            char* pst = wide_string_to_UTF8(parser.getErrorMessage());
            Scierror(999, "%s", pst);
            FREE(pst);

            delete parser.getTree();
            ThreadManagement::UnlockParser();
            return types::Function::Error;
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

        ConfigVariable::setExecutedFileID(iID);
    }
    else if (in[0]->isMacro() || in[0]->isMacroFile())
    {
        if (in[0]->isMacroFile())
        {
            //1st argument is a macro name, parse and execute it in the current environnement
            if (in[0]->getAs<types::MacroFile>()->parse() == false)
            {
                char* pstMacro = wide_string_to_UTF8(in[0]->getAs<types::MacroFile>()->getName().c_str());
                Scierror(999, _("%s: Unable to parse macro '%s'"), "exec", pstMacro);
                FREE(pstMacro);
                return types::Function::Error;
            }
            pMacro = in[0]->getAs<types::MacroFile>()->getMacro();
        }
        else //1st argument is a macro name, execute it in the current environnement
        {
            pMacro = in[0]->getAs<types::Macro>();
        }

        // unable for macro with varargin or varargout
        auto inputs = pMacro->getInputs();
        auto outputs = pMacro->getOutputs();
        if ((inputs->size() != 0 && (inputs->back()->getSymbol().getName() == L"varargin")) ||
                (outputs->size() != 0 && (outputs->back()->getSymbol().getName() == L"varargout")))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A macro without varargin and varargout expected.\n"), "exec", 1);
            return types::Function::Error;
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
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "exec", 1);
        return types::Function::Error;
    }

    if (pMacro)
    {
        //store the line number where is stored this macro in file.
        ConfigVariable::macroFirstLine_begin(pMacro->getFirstLine());
    }

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(promptMode);

    ast::SeqExp* pSeqExp = pExp->getAs<ast::SeqExp>();
    pSeqExp->setExecFrom(ast::SeqExp::EXEC);
    pSeqExp->setReturnable();
    std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());

    try
    {
        symbol::Context* pCtx = symbol::Context::getInstance();
        int scope = pCtx->getScopeLevel();
        int level = ConfigVariable::getRecursionLevel();
        try
        {
            pSeqExp->accept(*exec);
        }
        catch (const ast::RecursionException& /* re */)
        {
            //close opened scope during try
            while (pCtx->getScopeLevel() > scope)
            {
                pCtx->scope_end();
            }

            //decrease recursion to init value
            while (ConfigVariable::getRecursionLevel() > level)
            {
                ConfigVariable::where_end();
                ConfigVariable::decreaseRecursion();
            }

            //print msg about recursion limit and trigger an error
            wchar_t sz[1024];
            os_swprintf(sz, 1024, _W("Recursion limit reached (%d).\n").data(), ConfigVariable::getRecursionLimit());
            throw ast::InternalError(sz);
        }
    }
    catch (const ast::InternalAbort& ia)
    {
        closeFile(file, iID, wstFile, pExp);
        throw ia;
    }
    catch (const ast::InternalError& ie)
    {
        if (pMacro && ConfigVariable::getLastErrorFunction() == L"")
        {
            ConfigVariable::setLastErrorFunction(pMacro->getName());
        }

        if (bErrCatch == false)
        {
            closeFile(file, iID, wstFile, pExp);
            ConfigVariable::setPromptMode(oldVal);
            ConfigVariable::setExecutedFileID(0);
            throw ie;
        }

        ConfigVariable::resetWhereError();
        iErr = ConfigVariable::getLastErrorNumber();
    }

    //restore previous prompt mode
    ConfigVariable::setPromptMode(oldVal);
    if (bErrCatch)
    {
        out.push_back(new types::Double(iErr));
        //to lock last error information
        ConfigVariable::setLastErrorCall();
    }

    closeFile(file, iID, wstFile, pExp);
    return types::Function::OK;
}
