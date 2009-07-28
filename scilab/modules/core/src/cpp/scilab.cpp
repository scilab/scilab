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

#pragma comment(lib,"../../bin/libintl.lib")

#include <cstdio>
#include <iostream>
#include <string.h>

#include "timer.hxx"

extern "C"
{
	#include "SetScilabEnvironment.h"
	#include "prompt.h"
	#include "InitializeLocalization.h"
	#include "MALLOC.h"
	#include "setgetSCIpath.h"
	#include "inisci-c.h"
#ifdef _MSC_VER
	#include "../src/c/scilab_windows/getScilabDirectory.h"
#else
#endif
}

#include "exit_status.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "visitor.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "debugvisitor.hxx"
#include "configvariable.hxx"
//#include "setenvvar.hxx"
#include "funcmanager.hxx"

#define INTERACTIVE	-1

const char*	prog_name;
const char*	file_name;

timer _timer;

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

int InitializeEnvironnement(void);

/*
** Usage
**
** Display usage : options available in YaSp
*/
static void usage (void)
{
	std::cerr << "Usage: "<< prog_name << " [--parse-trace] [--display-tree] [--exec] [-f file] | --help" << std::endl;
	std::cerr << "--parse-trace : Display bison state machine evolution." << std::endl;
	std::cerr << "--display-tree : Display Syntax tree formated as understood scilab code." << std::endl;
	std::cerr << "--context-dump : Display what is stored in scilab at the end." << std::endl;
	std::cerr << "--timed : Enable timer." << std::endl;
	std::cerr << "--exec : Run the scilab code." << std::endl;
	std::cerr << "--debug : Print the AST nodes." << std::endl;
	std::cerr << "-f file : Batch mode on the given file." << std::endl;
	std::cerr << "-l lang : Change the language of scilab ( default : en_US )" << std::endl;
	std::cerr << "--help : Display this help." << std::endl;
}


/*
** Get Options
**
**
*/
static int	get_option (const int argc, char *argv[], int *_piFileIndex, int *_piLangIndex)
{
	int	i = 0;

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
		else if (!strcmp("-f", argv[i])) {
			i++;
			*_piFileIndex = i;
		}
		else if (!strcmp("-l", argv[i])) {
			i++;
			*_piLangIndex = i;
		}
	}

#ifdef DEBUG
	std::cerr << "File : " << argv[good] << std::endl;
#endif

	return 0;
}

/*
** HACK HACK HACK
*/

extern "C"
{
#include <stdio.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif
	extern char *TermReadAndProcess(void);
}

/*
** Parse
**
** Parse the given file and create the AST.
*/
static Parser::ParserStatus parseFileTask(void)
{
#ifdef DEBUG
	std::cerr << "*** Processing " << file_name << " file..." << std::endl;
#endif

	if (timed) { _timer.start(); }
	{
		Parser::getInstance()->parseFile(file_name, prog_name);
	}
	if (timed) { _timer.check("Parsing"); }

	return (Parser::getInstance()->getExitStatus());
}

/*
** Parse
**
** Parse the given command and create the AST.
*/
static Parser::ParserStatus parseCommandTask(char *command)
{
#ifdef DEBUG
	std::cerr << "*** Processing [" <<  command << "]..." << std::endl;
#endif

	if (timed) { _timer.start(); }
	{
		Parser::getInstance()->parse(command);
	}
	if (timed) { _timer.check("Parsing"); }

	return (Parser::getInstance()->getExitStatus());
}

/*
** Dump AST
**
** Display the AST in human readable format.
*/
static void dumpAstTask(void)
{
	if (timed) { _timer.start(); }
	{
		ast::DebugVisitor debugMe = *new ast::DebugVisitor();
		if (Parser::getInstance()->getTree())
		  {
		    Parser::getInstance()->getTree()->accept(debugMe);
		  }
	}
	if (timed) { _timer.check("AST Dump"); }
}

/*
** Pretty Print
**
** Pretty print the Stored AST.
*/
static void printAstTask(void)
{
	if (timed) { _timer.start(); }
	{
		ast::PrintVisitor printMe = *new ast::PrintVisitor(std::cout);
		Parser::getInstance()->getTree()->accept(printMe);
	}
	if (timed) { _timer.check("Pretty Print"); }
}


/*
** Exec Tree
**
** Execute the stored AST.
*/
static void execAstTask(void)
{
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
** Dump Stack Trace
**
** Display what is stored in scilab.
*/
static void dumpStackTask(void)
{
	if (timed) { _timer.start(); }
	{
		Context::getInstance()->print();
	}
	if (timed) { _timer.check("Dumping Stack"); }
}

/*
** -*- Batch Main -*-
*/
static int batchMain (void)
{
	/*
	** -*- PARSING -*-
	*/
	Parser::ParserStatus parseResult = parseFileTask();

	/*
	** -*- DUMPING TREE -*-
	*/
	if (dumpAst == true) { dumpAstTask(); }

	if (parseResult != Parser::Succeded)
	{
		return PARSE_ERROR;
	}

	/*
	** -*- PRETTY PRINT TREE -*-
	*/
	if (printAst == true) { printAstTask(); }

	/*
	** -*- EXECUTING TREE -*-
	*/
	if (execAst == true) { execAstTask(); }

	/*
	** -*- DUMPING STACK AFTER EXECUTION -*-
	*/
	if (dumpStack == true) { dumpStackTask(); }

#ifdef DEBUG
	std::cerr << "To end program press [ENTER]" << std::endl;
#endif

	return WELL_DONE;
}

/*
** -*- Interactive Main -*-
*/
static int interactiveMain (void)
{
	Parser::ParserStatus parseResult;
	bool exit = false;
	int pause = 0;

	std::cout << "-*- Yet Another Scilab Project -*-" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	while (!exit)
	{
		/* Display prompt */
/*
		std::cout << std::endl;
		std::cout << "YaSp --> ";

*/
		C2F(setprlev)(&pause);
		char *command = TermReadAndProcess();


		if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0)
		{
			exit = true;
			return 0;
		}

		if (strcmp(command, "") != 0)
		{
			/*
			** -*- PARSING -*-
			*/
			parseResult = parseCommandTask(command);

			/*
			** -*- DUMPING TREE -*-
			*/
			if (dumpAst == true) { dumpAstTask(); }

			if (parseResult == Parser::Succeded)
			{
				/*
				** -*- PRETTY PRINT TREE -*-
				*/
				if (printAst == true) { printAstTask(); }

				/*
				** -*- EXECUTING TREE -*-
				*/
				if (execAst == true) { execAstTask(); }

				/*
				** -*- DUMPING STACK AFTER EXECUTION -*-
				*/
				if (dumpStack == true) { dumpStackTask(); }
			}
			else
			  {
			    std::cerr << "Parser control : " << Parser::getInstance()->getControlStatus() << std::endl;
			  }
		}
	}
#ifdef DEBUG
	std::cerr << "To end program press [ENTER]" << std::endl;
#endif
	return WELL_DONE;
}

/*
** -*- MAIN -*-
*/
int main(int argc, char *argv[])
{
	int	iFileIndex = INTERACTIVE;
	int iLangIndex = 0;
	int iMainRet = 0;
	prog_name = argv[0];

	Parser::getInstance()->disableParseTrace();
	get_option(argc, argv, &iFileIndex, &iLangIndex);

	/* Scilab Startup */
	InitializeEnvironnement();

	/* set current language of scilab */
	FuncManager *pFM = new FuncManager();

	if (iFileIndex == INTERACTIVE)
	{
		file_name = "prompt";
		iMainRet = interactiveMain();
	}
	else
	{
		file_name = argv[iFileIndex];
		iMainRet = batchMain();
	}

	delete pFM;
	return iMainRet;
}

int InitializeEnvironnement(void)
{
	char *ScilabDirectory = NULL;
#ifdef _MSC_VER
	ScilabDirectory = getScilabDirectory(FALSE);
#else
	SetSci();
	ScilabDirectory = getSCIpath();
#endif
	if(ScilabDirectory)
	{
		ConfigVariable::getInstance()->set("SCI", ScilabDirectory);
		FREE(ScilabDirectory);
	}
	SetScilabEnvironment();
	InitializeLocalization();
	Add_All_Variables();
	return 0;
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
	Context::getInstance()->put(symbol::Symbol(_szName), *pVar);

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

