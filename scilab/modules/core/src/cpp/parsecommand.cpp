/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2019-2019 - ESI Group - Cedric Delamarre
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

static char* doParseCommand(const std::string& command, ast::Exp** tree, bool bDeleteTree)
{
    *tree = nullptr;
    ThreadManagement::LockParser();
    Parser parse;
    parse.parse(command.data());
    if (parse.getExitStatus() != Parser::Succeded)
    {
        char* msg = wide_string_to_UTF8(parse.getErrorMessage());
        ThreadManagement::UnlockParser();
        return msg;
    }
    ThreadManagement::UnlockParser();
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
    return doParseCommand(command, (ast::Exp**)tree, false);
}

char* checkCommand(const char* command)
{
    ast::Exp* pExp = nullptr;
    return doParseCommand(command, &pExp, true);
}

static wchar_t* doParseFileW(const std::wstring& file, const std::wstring& program, ast::Exp** tree, bool bDeleteTree)
{
    *tree = nullptr;
    ThreadManagement::LockParser();
    Parser parse;
    parse.parseFile(file.data(), program.data());
    if (parse.getExitStatus() != Parser::Succeded)
    {
        wchar_t* msg = os_wcsdup(parse.getErrorMessage());
        ThreadManagement::UnlockParser();
        return msg;
    }
    ThreadManagement::UnlockParser();
    *tree = parse.getTree();
    if(bDeleteTree && tree)
    {
        delete *tree;
        parse.setTree(nullptr);
    }

    return nullptr;
}

static char* doParseFile(const std::string& file, const std::string& program, ast::Exp** tree, bool bDeleteTree)
{
    char* pcError = nullptr;
    wchar_t* wcsFile = to_wide_string(file.data());
    wchar_t* wcsProgram = to_wide_string(program.data());
    wchar_t* wcsError = doParseFileW(wcsFile, wcsProgram, tree, bDeleteTree);
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
    return doParseFile(file, program, (ast::Exp**)tree, false);
}

char* checkFile(const char* file, const char* program)
{
    ast::Exp* pExp = nullptr;
    return doParseFile(file, program, &pExp, true);
}
