/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __PARSER_HXX__
#define  __PARSER_HXX__

#include <cstdio>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "parse.hxx"

#ifdef _MSC_VER
#if PARSE_EXPORTS
#define EXTERN_PARSE __declspec (dllexport)
#else
#define EXTERN_PARSE __declspec (dllimport)
#endif
#else
#define EXTERN_PARSE
#endif


class EXTERN_PARSE Parser
{
public:
    Parser()
    {
        _stop_on_first_error = false;
        _strict_mode = false;
        _parse_trace = false;
        _exit_status = Succeded;
        _control_status = AllControlClosed;
        _error_message = NULL;
        _the_program = NULL;
    }
    
    ~Parser()
    {
        // Do not delete Tree here.
    }

public:
    enum ParserStatus {
        Succeded ,
        Failed
    };
    
public:
    enum ControlStatus {
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
        WithinCell
    };

public:

    /** \brief parse the given file name */
    void parseFile(const std::string& name, const std::string& progName);

    /** \brief parse the given file command */
    void parse(char *command);

    /** \brief enable Bison trace mode */
    void setParseTrace(bool parseTrace) { _parse_trace = parseTrace; }
    bool getParseTrace(void) { return _parse_trace; }

    /** \brief free current tree */
    void freeTree();

    /** Setters / Getters
        \{ */

    ast::Exp* getTree(void) { return _the_program; }
    void setTree(ast::Exp* theProgram) { _the_program = theProgram; }

    ParserStatus getExitStatus(void) { return _exit_status; }
    void setExitStatus(ParserStatus exit_status) { _exit_status = exit_status; }

    ControlStatus getControlStatus(void) { return _control_status; }
    void setControlStatus(ControlStatus controlStatus) { _control_status = controlStatus; }
    
    char *getErrorMessage(void) { return const_cast<char *>(_error_message->c_str()); }
    void setErrorMessage(std::string *errorMessage) { _error_message = errorMessage; }

    void enableStrictMode(void) { _strict_mode = true; }
    void disableStrictMode(void) { _strict_mode = false; }

    bool stopOnFirstError(void) { return _stop_on_first_error; }
    void enableStopOnFirstError(void) { _stop_on_first_error = true; }
    void disableStopOnFirstError(void) { _stop_on_first_error = false; }
    /** \} */

private :
    const std::string* _file_name;
    const std::string* _prog_name;
    std::string* _error_message;
    bool _parse_trace;
    bool _strict_mode;
    bool _stop_on_first_error;
    ast::Exp* _the_program;
    ParserStatus _exit_status;
    ControlStatus _control_status;
};

#endif /* !__PARSER_HXX__ */
