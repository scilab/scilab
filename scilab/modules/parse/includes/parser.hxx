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

class Parser
{
private:
  Parser() { _exit_status = 0; }
  ~Parser()
  {
    delete _the_program;
  }

public:
  static Parser* getInstance(void);

public:

  /** \brief parse the given file name */
  void parseFile(const std::string& name, const std::string& progName);

  /** \brief enable Bison trace mode */
  void enableParseTrace(void);

  /** \brief disable Bison trace mode */
  void disableParseTrace(void);

  /** Setters / Getters
      \{ */
  const std::string* getFileName(void) { return _file_name; }
  void setFileName(const std::string& fileName) { _file_name = &fileName; }

  const std::string* getProgName(void) { return _prog_name; }
  void setProgName(const std::string& progName) { _prog_name = &progName; }

  ast::Exp* getTree(void) { return _the_program; }
  void setTree(ast::Exp* theProgram) { _the_program = theProgram; }

  int	getExitStatus(void) { return _exit_status; }
  void	setExitStatus(int exit_status) { _exit_status = exit_status; }
  /** \} */

  static void PrintError(std::string msg);

private :
  static Parser* me;
  const std::string* _file_name;
  const std::string* _prog_name;
  ast::Exp* _the_program;
  int _exit_status;
};

#endif /* !_PARSER_HH_ */
