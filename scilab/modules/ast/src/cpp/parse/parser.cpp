/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifdef __APPLE__
#ifndef HAVE_SYS_FMEMOPEN
#include "fmemopen.h"
#endif
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
#include "os_wfopen.h"
}

extern FILE*    yyin;
extern int      yyparse();
extern int      yydebug;
extern int      yylex_destroy();

void Parser::cleanup()
{
    yylex_destroy();
}

void Parser::parseFile(const std::wstring& fileName, const std::wstring& progName)
{
    try
    {
        ParserSingleInstance::parseFile(fileName, progName);
    }
    catch (const ast::InternalError& ie)
    {
        ParserSingleInstance::setTree(nullptr);
        ParserSingleInstance::setExitStatus(Parser::Failed);
    }

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
void ParserSingleInstance::parseFile(const std::wstring& fileName, const std::wstring& progName)
{
    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
    yyin = os_wfopen(fileName.c_str(), L"r");
#else
    char* pstTemp = wide_string_to_UTF8(fileName.c_str());
    yyin = fopen(pstTemp, "r");
    FREE(pstTemp);
#endif

    if (!yyin)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Cannot open file %ls.\n").c_str(), L"parser", fileName.c_str());
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

void Parser::parse(const wchar_t *command)
{
    char* pstCommand = wide_string_to_UTF8(command);
    parse(pstCommand);
    FREE(pstCommand);
}

bool Parser::stopOnFirstError(void)
{
    return ParserSingleInstance::stopOnFirstError();
}
void Parser::enableStopOnFirstError(void)
{
    ParserSingleInstance::enableStopOnFirstError();
}
void Parser::disableStopOnFirstError(void)
{
    ParserSingleInstance::disableStopOnFirstError();
}

#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <atomic>
#include <process.h>
FILE* fmemopen(void* buf, size_t len, const char* type)
{
    int fd;
    FILE* fp;
    char szFile[MAX_PATH];
    static std::atomic<unsigned int> uiCounter(0);
    char* tmpdir = getTMPDIR();
    sprintf(szFile, "%s\\command_%u_%u.temp", tmpdir, (unsigned int)_getpid(), uiCounter++);
    FREE(tmpdir);

    if (_sopen_s(&fd, szFile, _O_CREAT | _O_SHORT_LIVED | _O_TEMPORARY | _O_RDWR | _O_NOINHERIT, _SH_DENYRW, _S_IREAD | _S_IWRITE) != 0)
    {
        return NULL;
    }

    if (fd == -1)
    {
        return NULL;
    }

    fp = _fdopen(fd, "wt+");
    if (!fp)
    {
        _close(fd);
        return NULL;
    }

    fwrite(buf, len, 1, fp);
    rewind(fp);
    return fp;
}
#endif

/** \brief parse the given file command */
void ParserSingleInstance::parse(const char *command)
{
    size_t len = strlen(command);

    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 1;

    ParserSingleInstance::disableStrictMode();
    ParserSingleInstance::setFileName(L"prompt");
    ParserSingleInstance::setTree(nullptr);
    ParserSingleInstance::setExitStatus(Parser::Succeded);
    ParserSingleInstance::resetControlStatus();
    ParserSingleInstance::resetErrorMessage();

    yyin = fmemopen((void*)command, len, "r");
    if (!yyin)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Cannot create temporary buffer to parse the command.\n").c_str(), L"parser");
        ParserSingleInstance::appendErrorMessage(szError);
        ParserSingleInstance::setExitStatus(Parser::Failed);
        return;
    }

    yyparse();

    fclose(yyin);
}

/** \brief put the asked line in codeLine */
char *ParserSingleInstance::getCodeLine(int line, char **codeLine)
{
    int i = 0;

    // Store position of yyin to avoid side effects
    // in FLEX
    long lPos = ftell(yyin);
    rewind(yyin);
    /*
    ** WARNING : *codeLine will be allocated by getline
    ** so it must be manually freed !
    */
    for (i = 1 ; i <= line ; ++i)
    {
        if(fgets(*codeLine, 4096, yyin) == NULL)
        {
            break;
        }
    }

    fseek(yyin, lPos, SEEK_SET);
    return *codeLine;
}

std::wstring& ParserSingleInstance::getErrorMessage(void)
{
    return _error_message;
}

void ParserSingleInstance::appendErrorMessage(const std::wstring& message)
{
    if (ParserSingleInstance::stopOnFirstError() && _error_message.empty() == false)
    {
        return;
    }

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

std::wstring ParserSingleInstance::_file_name;
std::wstring ParserSingleInstance::_prog_name;
std::wstring ParserSingleInstance::_error_message;
bool ParserSingleInstance::_strict_mode = false;
bool ParserSingleInstance::_stop_on_first_error = true;
ast::Exp* ParserSingleInstance::_the_program = nullptr;
Parser::ParserStatus ParserSingleInstance::_exit_status = Parser::Succeded;
std::list<Parser::ControlStatus> ParserSingleInstance::_control_status;
FILE* ParserSingleInstance::fileLocker = nullptr;

