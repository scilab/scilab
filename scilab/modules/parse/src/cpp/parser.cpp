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
#endif

extern FILE*    yyin;
extern int      yyparse();
extern int      yydebug;

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
    if (getExitStatus() == Parser::Succeded) {
        this->setTree(ParserSingleInstance::getTree());
    }
  // FIXME : UNLOCK
}


/** \brief parse the given file name */
void ParserSingleInstance::parseFile(const std::string& fileName, const std::string& progName)
{
  yylloc.first_line = yylloc.last_line = 1;
  yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
	fopen_s(&yyin, fileName.c_str (), "r");
#else
  yyin = fopen(fileName.c_str (), "r");
#endif

  if (!yyin)
    {
      std::cerr << "*** Error -> cannot open `" << fileName << "`" << std::endl;
      exit (SYSTEM_ERROR);
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

void Parser::parse(char *command) {
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
    if (getExitStatus() == Parser::Succeded) {
        this->setTree(ParserSingleInstance::getTree());
    }
  // FIXME : UNLOCK
}

/** \brief parse the given file command */
void ParserSingleInstance::parse(char *command)
{
   char *codeLine = NULL;
   size_t len = 0;

  yylloc.first_line = yylloc.last_line = 1;
  yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
	TCHAR szFile[] = TEXT("command.temp");
	fopen_s(&yyin, "command.temp", "w");
	fwrite(command, 1, strlen(command), yyin);
	fclose(yyin);
	fopen_s(&yyin, "command.temp", "r");
#endif

#ifdef __APPLE__
	yyin = fopen("command.temp", "w");
	fwrite(command, 1, strlen(command), yyin);
	fclose(yyin);
	yyin = fopen("command.temp", "r");
#endif


#ifndef _MSC_VER
#ifndef __APPLE__
  yyin = fmemopen(command, strlen(command), "r");
#endif
#endif

  ParserSingleInstance::disableStrictMode();
  ParserSingleInstance::setFileName(*new std::string("prompt"));
  ParserSingleInstance::setExitStatus(Parser::Succeded);
  ParserSingleInstance::resetControlStatus();
  ParserSingleInstance::resetErrorMessage();

  yyparse();

  fclose(yyin);
#ifdef _MSC_VER
	DeleteFile(szFile);
#endif
}

/** \brief put the asked line in codeLine */
char *ParserSingleInstance::getCodeLine(int line, char **codeLine) {
#ifndef _MSC_VER
#ifndef __APPLE__
   size_t len = 0;
   int i = 0;

   rewind(yyin);
   /*
   ** WARNING : *codeLine will be allocated by getline
   ** so it must be manually freed !
   */
   for (i = 1 ; i <= line ; ++i)
     {
       getline(codeLine, &len, yyin);
     }
#endif
#endif
   return *codeLine;
}

char *ParserSingleInstance::getErrorMessage(void)
{
  return (char *)_error_message->c_str();
}

void ParserSingleInstance::appendErrorMessage(std::string message)
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

void Parser::freeTree()
{
	if(_the_program != NULL)
	{
		delete _the_program;
		_the_program = NULL;
	}
}

const std::string* ParserSingleInstance::_file_name = NULL;
const std::string* ParserSingleInstance::_prog_name = NULL;
std::string* ParserSingleInstance::_error_message = new std::string();
bool ParserSingleInstance::_strict_mode = false;
bool ParserSingleInstance::_stop_on_first_error = false;
ast::Exp* ParserSingleInstance::_the_program = NULL;
Parser::ParserStatus ParserSingleInstance::_exit_status = Parser::Succeded;
std::list<Parser::ControlStatus> *ParserSingleInstance::_control_status = new std::list<Parser::ControlStatus>();
