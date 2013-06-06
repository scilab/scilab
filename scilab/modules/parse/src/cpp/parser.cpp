/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <fstream>
#include <string>
#include <string.h>
#include "parser.hxx"
#include "parser_private.hxx"
#include "scilabexception.hxx"

#ifdef _MSC_VER
#include "windows.h"
#include "charEncoding.h"
#include "MALLOC.h"
#endif

extern "C"
{
#include "sci_tmpdir.h"
#include "Scierror.h"
#include "localization.h"
#include "os_swprintf.h"
#ifdef __APPLE__
#include "PATH_MAX.h"
#endif
}

extern FILE*    yyin;
extern int      yyparse();
extern int      yydebug;

void Parser::parseFile(const wstring& fileName, const wstring& progName)
{
    // Calling Parse state machine in C with global values
    // Must be locked to avoid concurrent access
    // FIXME : LOCK
    if (getParseTrace() == true)
    {
        ParserSingleInstance::enableParseTrace();
    }
    else
    {
        ParserSingleInstance::disableParseTrace();
    }
    ParserSingleInstance::parseFile(fileName, progName);
    this->setExitStatus(ParserSingleInstance::getExitStatus());
    this->setControlStatus(ParserSingleInstance::getControlStatus());
    if (getExitStatus() == Parser::Succeded)
    {
        this->setTree(ParserSingleInstance::getTree());
    }
    else
    {
        this->setErrorMessage(ParserSingleInstance::getErrorMessage());
    }
    // FIXME : UNLOCK
}


/** \brief parse the given file name */
void ParserSingleInstance::parseFile(const wstring& fileName, const wstring& progName)
{
    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
    _wfopen_s(&yyin, fileName.c_str(), L"r");
#else
    char* pstTemp = wide_string_to_UTF8(fileName.c_str());
    yyin = fopen(pstTemp, "r");
    FREE(pstTemp);
#endif

    if (!yyin)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Cannot open file %ls.\n"), L"parser", fileName.c_str());
        throw ast::ScilabError(szError, 999, *new Location());
    }


    ParserSingleInstance::disableStrictMode();
    //  Parser::getInstance()->enableStrictMode();
    ParserSingleInstance::setFileName(fileName);
    ParserSingleInstance::setProgName(progName);

    ParserSingleInstance::setExitStatus(Parser::Succeded);
    ParserSingleInstance::resetControlStatus();
    ParserSingleInstance::resetErrorMessage();
    yyparse();
    fclose(yyin);
}

void Parser::parse(wchar_t *command)
{
    // Calling Parse state machine in C with global values
    // Must be locked to avoid concurrent access
    // FIXME : LOCK
    if (getParseTrace() == true)
    {
        ParserSingleInstance::enableParseTrace();
    }
    else
    {
        ParserSingleInstance::disableParseTrace();
    }

    char* pstCommand = wide_string_to_UTF8(command);
    ParserSingleInstance::parse(pstCommand);
    this->setExitStatus(ParserSingleInstance::getExitStatus());
    this->setControlStatus(ParserSingleInstance::getControlStatus());
    if (getExitStatus() == Parser::Succeded)
    {
        this->setTree(ParserSingleInstance::getTree());
    }
    else
    {
        this->setErrorMessage(ParserSingleInstance::getErrorMessage());
    }

    if (getControlStatus() == AllControlClosed && get_last_token() != YYEOF)
    {
        //set parser last token to EOF
        scan_throw(YYEOF);
    }

    FREE(pstCommand);
    // FIXME : UNLOCK
}

/** \brief parse the given file command */
void ParserSingleInstance::parse(char *command)
{
    char *codeLine = NULL;
    size_t len = strlen(command);

    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
    char szFile[MAX_PATH];
    char* pstTmpDIr = getTMPDIR();
    sprintf(szFile, "%s\\%s", pstTmpDIr, "command.temp");
    FREE(pstTmpDIr);
    if (fileLocker)
    {
        fclose(fileLocker);
    }

    fopen_s(&yyin, szFile, "w");
    fwrite(command, sizeof(char), len, yyin);
    fclose(yyin);
    fopen_s(&yyin, szFile, "r");
#endif

#ifdef __APPLE__
    char szFile[PATH_MAX];
    char* pstTmpDIr = "/tmp";
    sprintf(szFile, "%s/%s", getTMPDIR(), "command.temp");
    //FREE(pstTmpDIr);
    if (fileLocker)
    {
        fclose(fileLocker);
    }
    yyin = fopen(szFile, "w");
    fwrite(command, 1, len, yyin);
    fclose(yyin);
    yyin = fopen(szFile, "r");
#endif


#ifndef _MSC_VER
#ifndef __APPLE__
    yyin = fmemopen(command, len, "r");
#endif
#endif

    ParserSingleInstance::disableStrictMode();
    ParserSingleInstance::setFileName(L"prompt");
    ParserSingleInstance::setExitStatus(Parser::Succeded);
    ParserSingleInstance::resetControlStatus();
    ParserSingleInstance::resetErrorMessage();

    yyparse();

    fclose(yyin);
#ifdef _MSC_VER
    DeleteFileA(szFile);
#endif

#ifdef _MSC_VER
    //reopen a file to prevents max file opened.
    fopen_s(&fileLocker, szFile, "w");
#endif
#ifdef __APPLE__
    fileLocker = fopen(szFile, "w");
#endif
}

/** \brief put the asked line in codeLine */
char *ParserSingleInstance::getCodeLine(int line, char **codeLine)
{
    size_t len = 0;
    int i = 0;

    rewind(yyin);
    /*
    ** WARNING : *codeLine will be allocated by getline
    ** so it must be manually freed !
    */
    for (i = 1 ; i <= line ; ++i)
    {
        fgets(*codeLine, 4096, yyin);
    }
    return *codeLine;
}

wstring *ParserSingleInstance::getErrorMessage(void)
{
    return _error_message;
}

void ParserSingleInstance::appendErrorMessage(wstring message)
{
    *_error_message += message;
}

/** \brief enable Bison trace mode */
void ParserSingleInstance::enableParseTrace(void)
{
    yydebug = 1;
}

/** \brief disable Bison trace mode */
void ParserSingleInstance::disableParseTrace(void)
{
    yydebug = 0;
}

wstring ParserSingleInstance::_file_name = L"";
wstring ParserSingleInstance::_prog_name = L"";
wstring* ParserSingleInstance::_error_message = new wstring();
bool ParserSingleInstance::_strict_mode = false;
bool ParserSingleInstance::_stop_on_first_error = false;
ast::Exp* ParserSingleInstance::_the_program = NULL;
Parser::ParserStatus ParserSingleInstance::_exit_status = Parser::Succeded;
std::list<Parser::ControlStatus> *ParserSingleInstance::_control_status = new std::list<Parser::ControlStatus>();
FILE* ParserSingleInstance::fileLocker = NULL;
