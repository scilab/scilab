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

#ifndef _PARSER_HH_
#define  _PARSER_HH_

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
private:
  Parser()
  {
    _stop_on_first_error = false;
    _strict_mode = false;
    _exit_status = Succeded;
    _control_status = new std::list<ControlStatus>();
    _error_message = new std::string();
  }
  ~Parser()
  {
    delete _the_program;
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
    WithinFunction
  };

public:
  static Parser* getInstance(void);

public:

  /** \brief parse the given file name */
  void parseFile(const std::string& name, const std::string& progName);

  /** \brief parse the given file command */
  void parse(char *command);

  /** \brief enable Bison trace mode */
  void enableParseTrace(void);

  /** \brief disable Bison trace mode */
  void disableParseTrace(void);

  /** Setters / Getters
      \{ */
  const std::string* getFileName(void) { return _file_name; }
  void setFileName(const std::string& fileName) { _file_name = &fileName; }

  // FIXME : Must return the real program name
  //const std::string* getProgName(void) { return _prog_name; }
  const std::string* getProgName(void) { return new std::string("Scilab6"); }
  void setProgName(const std::string& progName) { _prog_name = &progName; }

  ast::Exp* getTree(void) { return _the_program; }
  void setTree(ast::Exp* theProgram) { _the_program = theProgram; }

  ParserStatus	getExitStatus(void) { return _exit_status; }
  void	setExitStatus(ParserStatus exit_status) { _exit_status = exit_status; }

  char *getErrorMessage(void);
  void appendErrorMessage(std::string ostr);
  void resetErrorMessage(void) { _error_message->clear(); }

  ControlStatus getControlStatus(void) {
    if (!_control_status->empty())
      {
	return _control_status->front();
      }
    return AllControlClosed;
  }
  void pushControlStatus(ControlStatus control_status) { 
    //std::cout << "Push front : " << control_status << std::endl;
    _control_status->push_front(control_status); 
  }
  void popControlStatus(void) {
    if(!_control_status->empty())
      {
	//std::cout << "Pop front" << std::endl;
	//std::cout << "size = " << _control_status->size() << std::endl;
	_control_status->pop_front();
      }
  }
  void resetControlStatus(void) {
    _control_status->clear();
  }

  bool isStrictMode(void) { return _strict_mode; }
  void enableStrictMode(void) { _strict_mode = true; }
  void disableStrictMode(void) { _strict_mode = false; }

  bool stopOnFirstError(void) { return _stop_on_first_error; }
  void enableStopOnFirstError(void) { _stop_on_first_error = true; }
  void disableStopOnFirstError(void) { _stop_on_first_error = false; }
  /** \} */

  /*
  ** \brief This funnction returns the parsed code written
  ** at the given line.
  ** out = getCodeLine(10, codeline)
  ** When returning, out == *codeLine, _BUT_ out is allocated by a inner
  ** function and must be manually free.
  */
  char* getCodeLine(int line, char **codeLine);

  static void PrintError(std::string msg);

private :
  static Parser* me;
  const std::string* _file_name;
  const std::string* _prog_name;
  std::string* _error_message;
  bool _strict_mode;
  bool _stop_on_first_error;
  ast::Exp* _the_program;
  ParserStatus _exit_status;
  std::list<ControlStatus> *_control_status;
};

#endif /* !_PARSER_HH_ */
