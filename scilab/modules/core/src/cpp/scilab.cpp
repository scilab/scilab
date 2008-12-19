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
#include "configvariable.hxx"
#include "setenvvar.hxx"
#include "funcmanager.hxx"

const char*	prog_name;
const char*	file_name;


bool printAst = false;
bool execAst = false;
bool dumpAst = false;
bool dumpStack = false;
bool timed = false;

using symbol::Context;
using std::string;

void Add_i(void);
void Add_pi(void);
void Add_s(void);
void Add_z(void);
void Add_All_Variables(void);

void Add_Double_Constant(string _szName, double _dblReal, double _dblImg, bool _bComplex);
void Add_Poly_Constant(string _szName, string _szPolyVar, int _iRank, Double *_pdblReal);


void usage ()
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
/*
	//TEST TONIO

	std::cout.width(16);
	std::cout.precision(14);
	std::cout << 112345678.9012345678901234;
	std::cout << std::endl;


	return 0;
	//TEST TONIO
*/
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

		SetScilabEnvironment();
		FuncManager *pFM = new FuncManager();

		Add_All_Variables();
		ast::ExecVisitor *execMe = new ast::ExecVisitor();

		if (timed) { _timer.start(); }
		{
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
#ifdef DEBUG
	std::cerr << "To end program press [ENTER]" << std::endl;
#endif
	return WELL_DONE;
}

void Add_All_Variables(void)
{
	Add_pi();
	Add_i();
	Add_s();
	Add_z();
}

void Add_pi(void)
{
	Add_Double_Constant("%pi", 3.1415926535897931159980, 0, false);
}

void Add_i(void)
{
	Add_Double_Constant("%i", 0, 1, true);
}

void Add_s(void)
{
	Double dblCoef(1,2);
	dblCoef.val_set(0, 0, 0);
	dblCoef.val_set(0, 1, 1);

	Add_Poly_Constant("%s","s", 2, &dblCoef);
}

void Add_z(void)
{
	Double dblCoef(1,2);
	dblCoef.val_set(0, 0, 0);
	dblCoef.val_set(0, 1, 1);

	Add_Poly_Constant("%z","z", 2, &dblCoef);
}

void Add_Poly_Constant(string _szName, string _szPolyVar, int _iRank, Double *_pdbl)
{
	types::MatrixPoly *pVar = new types::MatrixPoly(_szPolyVar, 1, 1, &_iRank);
	Poly *poPoly = pVar->poly_get(0,0);
	poPoly->coef_set(_pdbl);
	Context::getInstance()->put(*new symbol::Symbol(_szName), *pVar);

/*	pVal->val_set(0,0,_dblReal,_dblImg);
	Context::getInstance()->put(*new symbol::Symbol(_szName), *pVal);
*/
}

void Add_Double_Constant(string _szName, double _dblReal, double _dblImg, bool _bComplex)
{
	types::Double* pVal = new types::Double(1,1,_bComplex);
	pVal->val_set(0,0,_dblReal,_dblImg);
	symbol::Context::getInstance()->put(*new symbol::Symbol(_szName), *pVal);
}

