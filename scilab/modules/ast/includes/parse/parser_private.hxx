/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __PARSER_PRIVATE_HXX__
#define __PARSER_PRIVATE_HXX__

#include "parser.hxx"

class EXTERN_AST ParserSingleInstance
{
public :
    static Parser::ControlStatus getControlStatus(void)
    {
        if (!_control_status.empty())
        {
            return _control_status.front();
        }
        return Parser::AllControlClosed;
    }

    static void pushControlStatus(Parser::ControlStatus control_status)
    {
        _control_status.push_front(control_status);
    }

    static void popControlStatus(void)
    {
        if (!_control_status.empty())
        {
            _control_status.pop_front();
        }
    }

    static void resetControlStatus(void)
    {
        _control_status.clear();
    }

    static Parser::ParserStatus getExitStatus(void)
    {
        return _exit_status;
    }
    static void setExitStatus(Parser::ParserStatus exit_status)
    {
        _exit_status = exit_status;
    }

    static ast::Exp* getTree(void)
    {
        return _the_program;
    }
    static void setTree(ast::Exp* theProgram)
    {
        _the_program = theProgram;
    }

    /*
    ** Parsing functions
    */
    static void parse(const char *command);
    static void parseFile(const std::wstring& fileName, const std::wstring& progName);

    /*
    ** Manage strict Mode
    */
    static bool isStrictMode(void)
    {
        return _strict_mode;
    }
    static void enableStrictMode(void)
    {
        _strict_mode = true;
    }
    static void disableStrictMode(void)
    {
        _strict_mode = false;
    }

    /*
    ** Manage error recovery mode
    */
    static bool stopOnFirstError(void)
    {
        return _stop_on_first_error;
    }
    static void enableStopOnFirstError(void)
    {
        _stop_on_first_error = true;
    }
    static void disableStopOnFirstError(void)
    {
        _stop_on_first_error = false;
    }

    /*
    ** Manage recovered tree mode
    */
    static bool recoverTreeOnError(void)
    {
        return _recover_tree_on_error;
    }
    static void enableTreeRecovery(void)
    {
        _recover_tree_on_error = true;
    }
    static void disableTreeRecovery(void)
    {
        _recover_tree_on_error = false;
    }

    static bool useRuntimeContext(void)
    {
        return _use_runtime_context;
    }
    static void enableRuntimeContext(void)
    {
        _use_runtime_context = true;
    }
    static void disableRuntimeContext(void)
    {
        _use_runtime_context = false;
    }

    /*
    ** Bison Debug management
    */
    static void enableParseTrace(void);
    static void disableParseTrace(void);

    /*
    ** File name management
    */
    static const std::wstring getFileName(void)
    {
        return _file_name;
    }
    static void setFileName(const std::wstring& fileName)
    {
        _file_name = fileName;
    }

    /*
    ** Program Name Management
    */
    static const std::wstring getProgName(void)
    {
        return L"Scilab";
    }
    static void setProgName(const std::wstring& progName)
    {
        _prog_name = progName;
    }

    /*
    ** Error Message management
    */
    static std::wstring& getErrorMessage(void);
    static void appendErrorMessage(const std::wstring& ostr);
    static void resetErrorMessage(void)
    {
        _error_message.clear();
    }

    static void releaseTmpFile();
    /*
    ** \brief This funnction returns the parsed code written
    ** at the given line.
    ** out = getCodeLine(10, codeline)
    ** When returning, out == *codeLine, _BUT_ out is allocated by a inner
    ** function and must be manually free.
    */
    static char* getCodeLine(int line, char **codeLine);

    static void PrintError(const std::wstring& msg);

private :
    static std::wstring _file_name;
    static std::wstring _prog_name;
    static std::wstring _error_message;
    static bool _strict_mode;
    static bool _stop_on_first_error;
    static bool _recover_tree_on_error;
    static bool _use_runtime_context;
    static ast::Exp* _the_program;
    static Parser::ParserStatus _exit_status;
    static std::list<Parser::ControlStatus> _control_status;
    static FILE* fileLocker;

};

#endif /* !__PARSER_PRIVATE_HXX__ */
