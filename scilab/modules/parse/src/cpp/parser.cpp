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

#include <fstream>
#include <string>
#include <string.h>
#include "parser.hxx"

#ifdef _MSC_VER
#include "windows.h"
#endif

extern FILE*	yyin;
extern int	yyparse ();
extern int	yydebug;

Parser* Parser::getInstance(void)
{
	if (me == 0)
	{
		me = new Parser();
	}
	return me;
}

/** \brief parse the given file name */
void Parser::parseFile(const std::string& fileName, const std::string& progName)
{
  yylloc.first_line = yylloc.last_line = 1;
  yylloc.first_column = yylloc.last_column = 1;
  yyin = fopen(fileName.c_str (), "r");

  if (!yyin)
    {
      std::cerr << "*** Error -> cannot open `" << fileName << "`" << std::endl;
      exit (SYSTEM_ERROR);
    }
  Parser::getInstance()->setFileName(fileName);
  Parser::getInstance()->setProgName(progName);

  Parser::getInstance()->setExitStatus(Succeded);
  yyparse();
  fclose(yyin);
}

/** \brief parse the given file command */
void Parser::parse(char *command)
{
  yylloc.first_line = yylloc.last_line = 1;
  yylloc.first_column = yylloc.last_column = 1;
#ifdef _MSC_VER
	TCHAR szFile[] = TEXT("command.temp");
	fopen_s(&yyin, "command.temp", "a+");
	fwrite(command, 1, strlen(command), yyin);
	fclose(yyin);
	fopen_s(&yyin, "command.temp", "r");
#else
  yyin = fmemopen(command, strlen(command), "r");
#endif

  Parser::getInstance()->setFileName("prompt");
  Parser::getInstance()->setProgName("prompt");

  Parser::getInstance()->setExitStatus(Succeded);
  yyparse();
  fclose(yyin);
#ifdef _MSC_VER
	DeleteFile(szFile);
#endif
	
}

/** \brief enable Bison trace mode */
void Parser::enableParseTrace(void)
{
  yydebug = 1;
}

/** \brief disable Bison trace mode */
void Parser::disableParseTrace(void)
{
  yydebug = 0;
}

Parser* Parser::me;
