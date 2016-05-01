/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

#ifndef __PARSER_HXX__
#define  __PARSER_HXX__

#include <cstdio>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "parse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "sci_malloc.h"
#include "dynlib_ast.h"
}

class EXTERN_AST Parser
{
public:
    Parser() :
        _strict_mode(false),
        _parse_trace(false),
        _exit_status(Succeded),
        _control_status(AllControlClosed),
        _the_program(NULL)
    {
    }

    ~Parser()
    {
        // Do not delete Tree here.
    }

public:
    enum ParserStatus
    {
        Succeded ,
        Failed
    };

public:
    enum ControlStatus
    {
        AllControlClosed,
        WithinFor,
        WithinWhile,
        WithinIf,
        WithinElse,
        WithinElseIf,
        WithinTry,
        WithinCatch,
        WithinFunction,
        WithinSelect,
        WithinSwitch,
        WithinCase,
        WithinOtherwise,
        WithinMatrix,
        WithinCell,
        WithinBlockComment,
        WithinDots
    };

public:
    /** \brief cleanup parser internal buffers */
    static void cleanup();

    /** \brief parse the given file name */
    void parseFile(const std::wstring& name, const std::wstring& progName);

    /** \brief parse the given file command */
    void parse(const char *command);
    void parse(const wchar_t *command);

    /** \brief enable Bison trace mode */
    void setParseTrace(bool parseTrace)
    {
        _parse_trace = parseTrace;
    }
    bool getParseTrace(void)
    {
        return _parse_trace;
    }

    /** Setters / Getters
        \{ */

    ast::Exp* getTree(void)
    {
        return _the_program;
    }
    void setTree(ast::Exp* theProgram)
    {
        _the_program = theProgram;
    }

    ParserStatus getExitStatus(void)
    {
        return _exit_status;
    }
    void setExitStatus(ParserStatus exit_status)
    {
        _exit_status = exit_status;
    }

    ControlStatus getControlStatus(void)
    {
        return _control_status;
    }
    void setControlStatus(ControlStatus controlStatus)
    {
        _control_status = controlStatus;
    }

    wchar_t *getErrorMessage(void)
    {
        return const_cast<wchar_t *>(_error_message.c_str());
    }
    void setErrorMessage(const std::wstring& errorMessage)
    {
        _error_message = errorMessage;
    }

    void enableStrictMode(void)
    {
        _strict_mode = true;
    }
    void disableStrictMode(void)
    {
        _strict_mode = false;
    }

    void releaseTmpFile();
    bool stopOnFirstError(void);
    void enableStopOnFirstError(void);
    void disableStopOnFirstError(void);

    /** \} */


private :
    const std::wstring _file_name;
    const std::wstring _prog_name;
    std::wstring _error_message;
    bool _strict_mode;
    bool _parse_trace;
    ParserStatus _exit_status;
    ControlStatus _control_status;
    ast::Exp* _the_program;
};

#endif /* !__PARSER_HXX__ */

