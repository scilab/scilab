/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2006-2008 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

//#define DEBUG

#include <cstdio>
#include <iostream>
#include <string.h>

#include "timer.hxx"

#include "exit_status.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "visitor.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "debugvisitor.hxx"

const char*	prog_name;
const char*	file_name;

bool printAst = false;
bool execAst = false;
bool dumpAst = false;
bool dumpStack = false;
bool timed = false;

using symbol::Context;
using std::string;

void     usage ()
{
	std::cerr << "Usage: "<< prog_name << " [--parse-trace] [--display-tree] [--exec] file | --help" << std::endl;
	std::cerr << "--parse-trace : Display bison state machine evolution." << std::endl;
	std::cerr << "--display-tree : Display Syntax tree formated as understood scilab code." << std::endl;
	std::cerr << "--context-dump : Display what is stored in scilab at the end." << std::endl;
	std::cerr << "--timed : Enable timer." << std::endl;
	std::cerr << "--exec : Run the scilab code." << std::endl;
	std::cerr << "--debug : Print the AST nodes." << std::endl;
	std::cerr << "--help : Display this help." << std::endl;
}

int	get_option (const int argc, char *argv[])
{
	int	i = 0;
	int	good = 0;

#ifdef DEBUG
	std::cerr << "-*- Getting Options -*-"<< std::endl;
#endif

	for (i = 1; i < argc; ++i) {
		if (!strcmp("--parse-trace", argv[i])) {
			Parser::getInstance()->enableParseTrace();
		}
		else if (!strcmp("--display-tree", argv[i])) {
			printAst = true;
		}
		else if (!strcmp("--help", argv[i])) {
			usage ();
			exit (WELL_DONE);
		}
		else if (!strcmp("--debug", argv[i])) {
			dumpAst = true;
		}
		else if (!strcmp("--exec", argv[i])) {
			execAst = true;
		}
		else if (!strcmp("--context-dump", argv[i])) {
			dumpStack = true;
		}
		else if (!strcmp("--timed", argv[i])) {
			timed = true;
		}
		else if (!strncmp ("--", argv[i], 2)) {
			usage ();
			exit (SYSTEM_ERROR);
		}
		else
			good = i;
	}

#ifdef DEBUG
	std::cerr << "File : " << argv[good] << std::endl;
#endif
	if (good == 0)
	{
		usage();
		exit(SYSTEM_ERROR);
	}
	return good;
}

int	main (int argc, char *argv[])
{
	int	i;
	timer _timer;
	prog_name = argv[0];

	Parser::getInstance()->disableParseTrace();
	i = get_option(argc, argv);

	file_name = argv[i];

	/*
	** -*- PARSING -*-
	** {
	*/
#ifdef DEBUG
	std::cerr << "*** Processing " << file_name << " file..." << std::endl;
#endif

	if (timed) { _timer.start(); }
	{
		Parser::getInstance()->parseFile(file_name, prog_name);
	}
	if (timed) { _timer.check("Parsing"); }

	if (Parser::getInstance()->getExitStatus() != WELL_DONE)
	{
		return (Parser::getInstance()->getExitStatus());
	}
#ifdef DEBUG
	std::cerr << "*** Lexing/Parsing succesfull !!!" << std::endl;
#endif
	/*
	** }
	*/


	/*
	** -*- DUMPING TREE -*-
	** {
	*/
	if (dumpAst == true)
	{
		if (timed) { _timer.start(); }
		{
			ast::DebugVisitor debugMe = *new ast::DebugVisitor();
			Parser::getInstance()->getTree()->accept(debugMe);
		}
		if (timed) { _timer.check("AST Dump"); }
	}
	/*
	** }
	*/

	/*
	** -*- PRETTY PRINT TREE -*-
	** {
	*/
	if (printAst == true)
	{
		if (timed) { _timer.start(); }
		{
			ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);
			Parser::getInstance()->getTree()->accept(printMe);
		}
		if (timed) { _timer.check("Pretty Print"); }
	}
	/*
	** }
	*/

	/*
	** -*- EXECUTING TREE -*-
	** {
	*/
	if (execAst == true)
	{
		if (timed) { _timer.start(); }
		{
 			types::Double* Img = new types::Double(1,1,true);
			Img->val_set(0,0,0,1);
			Context::getInstance()->put(*new symbol::Symbol("%i"), *Img);

			ast::ExecVisitor *execMe = new ast::ExecVisitor();
			try
			{
				Parser::getInstance()->getTree()->accept(*execMe);
			}
			catch(string sz)
			{
				std::cerr << sz << std::endl;
			}
			delete execMe;
		}
		if (timed) { _timer.check("Execute AST"); }
	}
	/*
	** }
	*/

	/*
	** -*- DUMPING STACK AFTER EXECUTION -*-
	** {
	*/
	if (dumpStack == true)
	{
		if (timed) { _timer.start(); }
		{
			Context::getInstance()->print();
		}
		if (timed) { _timer.check("Dumping Stack"); }
	}
	/*
	** }
	*/

	std::cout << "To end program press [ENTER]" << std::endl;
	getchar();
	return WELL_DONE;
}
