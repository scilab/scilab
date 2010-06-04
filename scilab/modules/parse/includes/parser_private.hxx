/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __PARSER_PRIVATE_HXX__
#define __PARSER_PRIVATE_HXX__

#include "parser.hxx"

class ParserSingleInstance {
public :
    static Parser::ControlStatus getControlStatus(void) {
        if (!_control_status->empty())
        {
            return _control_status->front();
        }
        return Parser::AllControlClosed;
    }
    
    static void pushControlStatus(Parser::ControlStatus control_status) { 
        //std::cout << "Push front : " << control_status << std::endl;
        _control_status->push_front(control_status); 
    }
    
    static void popControlStatus(void) {
        if(!_control_status->empty())
        {
            //std::cout << "Pop front" << std::endl;
            //std::cout << "size = " << _control_status->size() << std::endl;
            _control_status->pop_front();
        }
    }
    
    static void resetControlStatus(void) {
        _control_status->clear();
    }

    static Parser::ParserStatus getExitStatus(void) { return _exit_status; }
    static void setExitStatus(Parser::ParserStatus exit_status) { _exit_status = exit_status; }

    static ast::Exp* getTree(void) { return _the_program; }
    static void setTree(ast::Exp* theProgram) { _the_program = theProgram; }

    /*
    ** Parsing functions
    */
    static void parse(char *command);
    static void parseFile(const std::string& fileName, const std::string& progName);

    /*
    ** Manage strict Mode
    */
    static bool isStrictMode(void) { return _strict_mode; }
    static void enableStrictMode(void) { _strict_mode = true; }
    static void disableStrictMode(void) { _strict_mode = false; }

    /*
    ** Manage error recovery mode
    */
    static bool stopOnFirstError(void) { return _stop_on_first_error; }
    static void enableStopOnFirstError(void) { _stop_on_first_error = true; }
    static void disableStopOnFirstError(void) { _stop_on_first_error = false; }

    /*
    ** Bison Debug management
    */
    static void enableParseTrace(void);
    static void disableParseTrace(void);

    /*
    ** File name management
    */
    static const std::string* getFileName(void) { return _file_name; }
    static void setFileName(const std::string& fileName) { _file_name = &fileName; }

    /*
    ** Program Name Management
    */
    static const std::string* getProgName(void) { return new std::string("Scilab6"); }
    static void setProgName(const std::string& progName) { _prog_name = &progName; }

    /*
    ** Error Message management
    */
    static char *getErrorMessage(void);
    static void appendErrorMessage(std::string ostr);
    static void resetErrorMessage(void) { _error_message->clear(); }   

    /*
    ** \brief This funnction returns the parsed code written
    ** at the given line.
    ** out = getCodeLine(10, codeline)
    ** When returning, out == *codeLine, _BUT_ out is allocated by a inner
    ** function and must be manually free.
    */
    static char* getCodeLine(int line, char **codeLine);

    static void PrintError(std::string msg);

private :
    static const std::string* _file_name;
    static const std::string* _prog_name;
    static std::string* _error_message;
    static bool _strict_mode;
    static bool _stop_on_first_error;
    static ast::Exp* _the_program;
    static Parser::ParserStatus _exit_status;
    static std::list<Parser::ControlStatus> *_control_status;
        
};

#endif /* !__PARSER_PRIVATE_HXX__ */
