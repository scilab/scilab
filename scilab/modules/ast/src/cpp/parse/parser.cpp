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

#ifdef _MSC_VER
#include "windows.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#endif

extern "C"
{
#include "sci_tmpdir.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#ifdef __APPLE__
#include "PATH_MAX.h"
#endif
}

extern FILE*    yyin;
extern int      yyparse();
extern int      yydebug;
extern int      yylex_destroy();

void Parser::cleanup()
{
    yylex_destroy();
}

void Parser::parseFile(const std::string& fileName, const std::string& progName)
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

    if (getExitStatus() != Parser::Succeded)
    {
        delete ParserSingleInstance::getTree();
        ParserSingleInstance::setTree(nullptr);
    }

    // FIXME : UNLOCK
}


/** \brief parse the given file name */
void ParserSingleInstance::parseFile(const std::string& fileName, const std::string& progName)
{
    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
    fopen_s(&yyin, fileName.c_str(), "r");
#else
    yyin = fopen(fileName.c_str(), "r");
#endif

    if (!yyin)
    {
        char szError[bsiz];
        os_sprintf(szError, _("%s: Cannot open file %s.\n"), "parser", fileName.c_str());
        throw ast::InternalError(szError);
    }


    ParserSingleInstance::disableStrictMode();
    //  Parser::getInstance()->enableStrictMode();
    ParserSingleInstance::setFileName(fileName);
    ParserSingleInstance::setProgName(progName);

    ParserSingleInstance::setTree(nullptr);
    ParserSingleInstance::setExitStatus(Parser::Succeded);
    ParserSingleInstance::resetControlStatus();
    ParserSingleInstance::resetErrorMessage();
    yyparse();
    fclose(yyin);
}

void Parser::parse(const char *command)
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

    ParserSingleInstance::parse(command);
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

    if (getExitStatus() != Parser::Succeded)
    {
        delete ParserSingleInstance::getTree();
        ParserSingleInstance::setTree(nullptr);
    }

    // FIXME : UNLOCK
}

/** \brief parse the given file command */
void ParserSingleInstance::parse(const char *command)
{
    size_t len = strlen(command);

    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
    char szFile[MAX_PATH];
    char* pstTmpDIr = getTMPDIR();
    os_sprintf(szFile, "%s\\%s", pstTmpDIr, "command.temp");
    FREE(pstTmpDIr);
    if (fileLocker)
    {
        fclose(fileLocker);
        fileLocker = nullptr;
    }

    errno_t err;
    err = fopen_s(&yyin, szFile, "w");
    if (err)
    {
        ParserSingleInstance::setExitStatus(Parser::Failed);
        ParserSingleInstance::resetErrorMessage();
        char szError[bsiz];
        os_sprintf(szError, _("%s: Cannot open file %s.\n"), "parser", szFile);
        appendErrorMessage(szError);
        return;
    }

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
        fileLocker = nullptr;
    }
    yyin = fopen(szFile, "w");
    fwrite(command, 1, len, yyin);
    fclose(yyin);
    yyin = fopen(szFile, "r");
#endif


#ifndef _MSC_VER
#ifndef __APPLE__
    yyin = fmemopen((void*)command, len, "r");
#endif
#endif

    ParserSingleInstance::disableStrictMode();
    ParserSingleInstance::setFileName("prompt");
    ParserSingleInstance::setTree(nullptr);
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

std::string& ParserSingleInstance::getErrorMessage(void)
{
    return _error_message;
}

void ParserSingleInstance::appendErrorMessage(const std::string& message)
{
    _error_message += message;
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

void Parser::releaseTmpFile()
{
    ParserSingleInstance::releaseTmpFile();
}

void ParserSingleInstance::releaseTmpFile()
{
    if (fileLocker)
    {
        //fclose(fileLocker);
        //fileLocker = nullptr;
    }
}

std::string ParserSingleInstance::_file_name;
std::string ParserSingleInstance::_prog_name;
std::string ParserSingleInstance::_error_message;
bool ParserSingleInstance::_strict_mode = false;
bool ParserSingleInstance::_stop_on_first_error = false;
ast::Exp* ParserSingleInstance::_the_program = nullptr;
Parser::ParserStatus ParserSingleInstance::_exit_status = Parser::Succeded;
std::list<Parser::ControlStatus> ParserSingleInstance::_control_status;
FILE* ParserSingleInstance::fileLocker = nullptr;

