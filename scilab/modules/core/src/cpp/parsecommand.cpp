/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2019-2019 - ESI Group - Cedric Delamarre
 *  Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "threadmanagement.hxx"
#include "parser.hxx"

extern "C"
{
#include "parsecommand.h"
}

namespace
{
class ParserGuard
{
public:
    ParserGuard(Parser& parser, bool recoverTreeOnError, bool useRuntimeContext)
        : m_parser(parser),
          m_previousStopOnFirstError(true),
          m_previousUseRuntimeContext(true)
    {
        ThreadManagement::LockParser();
        m_previousStopOnFirstError = parser.stopOnFirstError();
        m_previousUseRuntimeContext = parser.useRuntimeContext();
        if (recoverTreeOnError)
        {
            m_parser.enableTreeRecovery();
            m_parser.disableStopOnFirstError();
        }
        if (useRuntimeContext)
        {
            m_parser.enableRuntimeContext();
        }
        else
        {
            m_parser.disableRuntimeContext();
        }
    }

    ~ParserGuard()
    {
        if (m_previousStopOnFirstError)
        {
            m_parser.enableStopOnFirstError();
        }
        else
        {
            m_parser.disableStopOnFirstError();
        }
        if (m_previousUseRuntimeContext)
        {
            m_parser.enableRuntimeContext();
        }
        else
        {
            m_parser.disableRuntimeContext();
        }
        ThreadManagement::UnlockParser();
    }

private:
    Parser& m_parser;
    bool m_previousStopOnFirstError;
    bool m_previousUseRuntimeContext;

    ParserGuard(const ParserGuard&) = delete;
    ParserGuard& operator=(const ParserGuard&) = delete;
};
}

static char* doParseCommand(const std::string& command, ast::Exp** tree, bool bDeleteTree,
    bool recoverTreeOnError, bool useRuntimeContext = true)
{
    *tree = nullptr;
    Parser parse;
    ParserGuard guard(parse, recoverTreeOnError, useRuntimeContext);

    parse.parse(command.data());
    if (parse.getExitStatus() != Parser::Succeded)
    {
        char* msg = wide_string_to_UTF8(parse.getErrorMessage());
        *tree = parse.getTree();
        if(bDeleteTree && *tree)
        {
            delete *tree;
            parse.setTree(nullptr);
            *tree = nullptr;
        }

        return msg;
    }

    *tree = parse.getTree();
    if(bDeleteTree && *tree)
    {
        delete *tree;
        parse.setTree(nullptr);
    }

    return nullptr;
}

char* parseCommand(const char* command, void** tree)
{
    return doParseCommand(command, (ast::Exp**)tree, false, false);
}

char* parseCommandWithRecovery(const char* command, void** tree)
{
    return doParseCommand(command, (ast::Exp**)tree, false, true);
}

char* parseCommandWithRecoveryWithoutRuntime(const char* command, void** tree)
{
    return doParseCommand(command, (ast::Exp**)tree, false, true, false);
}

char* checkCommand(const char* command)
{
    ast::Exp* pExp = nullptr;
    return doParseCommand(command, &pExp, true, false);
}

static wchar_t* doParseFileW(const std::wstring& file, const std::wstring& program, ast::Exp** tree, bool bDeleteTree, bool recoverTreeOnError)
{
    wchar_t* msg = nullptr;
    *tree = nullptr;
    Parser parse;
    ParserGuard guard(parse, recoverTreeOnError, true);

    parse.parseFile(file.data(), program.data());
    if (parse.getExitStatus() != Parser::Succeded)
    {
        msg = os_wcsdup(parse.getErrorMessage());
    }

    *tree = parse.getTree();
    if(bDeleteTree && tree)
    {
        delete *tree;
        parse.setTree(nullptr);
    }

    return msg;
}

static char* doParseFile(const std::string& file, const std::string& program, ast::Exp** tree, bool bDeleteTree, bool recoverTreeOnError)
{
    char* pcError = nullptr;
    wchar_t* wcsFile = to_wide_string(file.data());
    wchar_t* wcsProgram = to_wide_string(program.data());
    wchar_t* wcsError = doParseFileW(wcsFile, wcsProgram, tree, bDeleteTree, recoverTreeOnError);
    FREE(wcsFile);
    FREE(wcsProgram);
    if (wcsError)
    {
        pcError = wide_string_to_UTF8(wcsError);
        FREE(wcsError);
    }

    return pcError;
}

char* parseFile(const char* file, const char* program, void** tree)
{
    return doParseFile(file, program, (ast::Exp**)tree, false, false);
}

char* parseFileWithRecovery(const char* file, const char* program, void** tree)
{
    return doParseFile(file, program, (ast::Exp**)tree, false, true);
}

char* checkFile(const char* file, const char* program)
{
    ast::Exp* pExp = nullptr;
    return doParseFile(file, program, &pExp, true, false);
}
