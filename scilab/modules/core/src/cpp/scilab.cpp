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

extern "C"
{
#ifndef _MSC_VER
#include <unistd.h>
#endif

	//#include "SetScilabEnvironment.h"
#include "prompt.h"
#include "localization.h"
#include "InitializeLocalization.h"
#include "MALLOC.h"
#include "setgetSCIpath.h"
#include "inisci-c.h"
#include "scilabmode.h"
#ifdef _MSC_VER
#include "../src/c/scilab_windows/getScilabDirectory.h"
#endif
#include "ConsoleRead.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../jvm/includes/InitializeJVM.h"
#include "InitializeCore.h"
#include "../../../shell/includes/InitializeShell.h"
#include "../../../console/includes/InitializeConsole.h"
#include "../../../tclsci/includes/InitializeTclTk.h"
#include "../../../localization/includes/InitializeLocalization.h"
#include "../../../graphics/includes/graphicModuleLoad.h"
#include "../../../jvm/includes/InitializeJVM.h"
#ifdef _MSC_VER
#include "../../../windows_tools/includes/InitializeWindows_tools.h"
#endif
#include "../../../gui/includes/InitializeGUI.h"
#include "../../../string/includes/InitializeString.h"
#include "scilabmode.h"
#include "SetScilabEnvironment.h"
#include "../../../jvm/includes/loadBackGroundClassPath.h"
	/*
	** HACK HACK HACK
	*/
	extern char *TermReadAndProcess(void);
}

#include "yaspio.hxx"
#include "tasks.hxx"
#include "exit_status.hxx"
#include "parser.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "context.hxx"
//#include "setenvvar.hxx"
#include "funcmanager.hxx"

#define INTERACTIVE	-1

const char*	prog_name;
const char*	file_name;

bool printAst = false;
bool execAst = true;
bool dumpAst = false;
bool dumpStack = false;
bool timed = false;
bool consoleMode = false;

using symbol::Context;
using std::string;

void Add_i(void);
void Add_pi(void);
void Add_s(void);
void Add_z(void);
void Add_true(void);
void Add_false(void);
void Add_All_Variables(void);

void Add_Double_Constant(string _szName, double _dblReal, double _dblImg, bool _bComplex);
void Add_Poly_Constant(string _szName, string _szPolyVar, int _iRank, Double *_pdblReal);
void Add_Boolean_Constant(string _szName, bool _bBool);

int InitializeEnvironnement(void);
bool execScilabStart(void);

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
	std::cerr << "--no-exec : Do not run the scilab code." << std::endl;
	std::cerr << "--debug : Print the AST nodes." << std::endl;
	std::cerr << "-f file : Batch mode on the given file." << std::endl;
	std::cerr << "-l lang : Change the language of scilab ( default : en_US )" << std::endl;
	std::cerr << "-nw : Enable console mode" << std::endl;
	std::cerr << "-nwni : Enable console mode" << std::endl;
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
		else if (!strcmp("--no-exec", argv[i])) {
			execAst = false;
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
		else if (!strcmp("-nw", argv[i]) || !strcmp("-nwni", argv[i])) {
			consoleMode = true;
			setScilabMode(SCILAB_NWNI);
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
#include "scilabmode.h"
	extern char *TermReadAndProcess(void);
	extern void ConsolePrintf(char*);
}


/*
** -*- Batch Main -*-
*/
static int batchMain (void)
{
	/*
	** -*- PARSING -*-
	*/
	Parser::ParserStatus parseResult = parseFileTask(timed, file_name, prog_name);

	/*
	** -*- DUMPING TREE -*-
	*/
	if (dumpAst == true) { dumpAstTask(timed); }

	if (parseResult != Parser::Succeded)
	{
		YaspWrite(Parser::getInstance()->getErrorMessage());
		return PARSE_ERROR;
	}

	/*
	** -*- PRETTY PRINT TREE -*-
	*/
	if (printAst == true) { printAstTask(timed); }

	/*
	** -*- EXECUTING TREE -*-
	*/
	if (execAst == true) { execAstTask(timed); }

	/*
	** -*- DUMPING STACK AFTER EXECUTION -*-
	*/
	if (dumpStack == true) { dumpStackTask(timed); }

#ifdef DEBUG
	std::cerr << "To end program press [ENTER]" << std::endl;
#endif

	return WELL_DONE;
}

static void banner()
{
#define SCI_VERSION_STRING "scilab-branch-YaSp"
	int i;
	char *line = "        ___________________________________________        ";
	int startVersion = (int)(floor((double)(strlen(line)/2)) - floor((double)(strlen(SCI_VERSION_STRING)/2)));

	YaspWrite(line);
	YaspWrite("\n");

	/* To center the version name */
	for( i=0 ; i<startVersion ; i++ )
	{
		YaspWrite(" ");
	}

	YaspWrite(SCI_VERSION_STRING);
	YaspWrite("\n\n");

	YaspWrite(_("                 Consortium Scilab (DIGITEO)\n"));

	YaspWrite(_("               Copyright (c) 1989-2009 (INRIA)\n"));
	YaspWrite(_("               Copyright (c) 1989-2007 (ENPC)\n"));
	YaspWrite(line);
	YaspWrite("\n");
	YaspWrite("\n");
	YaspWrite("           -*- THIS IS SCILAB 6.0 aka YaSp -*-\n");
	YaspWrite("\n");
	YaspWrite(line);
	YaspWrite("\n");
}

/*
** -*- stateView
** Used to show parser state.
** Find if we are stuck within some control structure.
*/

static void stateShow(Parser::ControlStatus status)
{
	switch (status)
	{
	case Parser::WithinFor : SetTemporaryPrompt("-for->"); break;
	case Parser::WithinWhile : SetTemporaryPrompt("-while->"); break;
	case Parser::WithinIf : SetTemporaryPrompt("-if->"); break;
	case Parser::WithinElse : SetTemporaryPrompt("-else->"); break;
	case Parser::WithinElseIf : SetTemporaryPrompt("-elseif->"); break;
	case Parser::WithinTry : SetTemporaryPrompt("-try->"); break;
	case Parser::WithinCatch : SetTemporaryPrompt("-catch->"); break;
	case Parser::WithinFunction : SetTemporaryPrompt("-function->"); break;
	case Parser::WithinSelect : SetTemporaryPrompt("-select->"); break;
	case Parser::WithinCase : SetTemporaryPrompt("-case->"); break;
	case Parser::AllControlClosed : SetTemporaryPrompt(SCIPROMPT); break;
	}
}

/*
** -*- Interactive Main -*-
*/
static int interactiveMain (void)
{
	Parser::ParserStatus parseResult;
	bool exit = false;
	int pause = 0;
	char *command = NULL;
	Parser* pParser = Parser::getInstance();

	banner();

	while (!exit)
	{
	  // Show Parser Sate before prompt
	  stateShow(pParser->getControlStatus());

		//set prompt value
		C2F(setprlev)(&pause);

		if (pParser->getControlStatus() == Parser::AllControlClosed) 
		{
			if(command)
			{
				FREE(command);
				command = NULL;
			}
			command = YaspRead();
		}
		else
		{
			char* pstRead = YaspRead();
			//+1 for null termination and +1 for '\n'
			size_t iLen = strlen(command) + strlen(pstRead) + 2;
			char* pstNewCommand = (char*)MALLOC(iLen * sizeof(char));
#ifdef _MSC_VER
			sprintf_s(pstNewCommand, iLen, "%s\n%s", command, pstRead);
#else
			sprintf(pstNewCommand, "%s\n%s", command, pstRead);
#endif
			FREE(pstRead);
			FREE(command);
			command = pstNewCommand;
		}

		//std::cout << "---" << std::endl << "Command = " << std::endl;
		//std::cout << command << std::endl;
		//std::cout << "---" << std::endl;
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
			parseResult = parseCommandTask(timed, command);

			/*
			** -*- DUMPING TREE -*-
			*/
			if (dumpAst == true) { dumpAstTask(timed); }

			if (parseResult == Parser::Succeded)
			{
				/*
				** -*- PRETTY PRINT TREE -*-
				*/
				if (printAst == true) { printAstTask(timed); }

				/*
				** -*- EXECUTING TREE -*-
				*/
				if (execAst == true) { execAstTask(timed); }

				/*
				** -*- DUMPING STACK AFTER EXECUTION -*-
				*/
				if (dumpStack == true) { dumpStackTask(timed); }
			}
			else if(parseResult == Parser::Failed && pParser->getControlStatus() == Parser::AllControlClosed)
			{
				YaspWrite(pParser->getErrorMessage());
				//std::cerr << "Parser control : " << pParser->getControlStatus() << std::endl;
			}
		}
	}
#ifdef DEBUG
	std::cerr << "To end program press [ENTER]" << std::endl;
#endif
	return WELL_DONE;
}

static void TermPrintf(char *text)
{
	std::cout << text;
}

/*
** -*- MAIN -*-
*/
int main(int argc, char *argv[])
{
	int iFileIndex = INTERACTIVE;
	int iLangIndex = 0;
	int iMainRet = 0;
	prog_name = argv[0];

	setScilabMode(SCILAB_STD);
	Parser::getInstance()->disableParseTrace();
	get_option(argc, argv, &iFileIndex, &iLangIndex);

	if (consoleMode)
	{
		setYaspInputMethod(&TermReadAndProcess);
		setYaspOutputMethod(&TermPrintf);
	}
	else
	{
		setYaspInputMethod(&ConsoleRead);
		setYaspOutputMethod(&ConsolePrintf);
	}

	/* Scilab Startup */
	InitializeEnvironnement();

	InitializeString();

	InitializeLocalization();

#ifdef _MSC_VER
	InitializeWindows_tools();
#endif

	InitializeCore();

	InitializeShell();

	if ( 
		!consoleMode ) 
	{
		/* bug 3702 */
		/* tclsci creates a TK window on Windows */
		/* it changes focus on previous windows */
		/* we put InitializeTclTk before InitializeGUI */

		//InitializeTclTk();
		InitializeJVM();
		InitializeGUI();

		/* create needed data structure if not already created */
		loadGraphicModule() ;

		/* Standard mode -> init Java Console */
		//if ( !consoleMode ) 
		{
			/* Initialize console: lines... */
			InitializeConsole();
		}

		loadBackGroundClassPath();
	}

	/* set current language of scilab */
	FuncManager *pFM = new FuncManager();

	//execute scilab.start
	//execScilabStartTask();



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

	//close main scope
	symbol::Context::getInstance()->scope_end();
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
	Add_true();
	Add_false();
}

void Add_false(void)
{
	Add_Boolean_Constant("%f", false);
}

void Add_true(void)
{
	Add_Boolean_Constant("%t", true);
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
	Context::getInstance()->put(_szName, *pVar);
}

void Add_Double_Constant(string _szName, double _dblReal, double _dblImg, bool _bComplex)
{
	types::Double* pVal = new types::Double(1,1,_bComplex);
	pVal->val_set(0,0,_dblReal,_dblImg);
	symbol::Context::getInstance()->put(_szName, *pVal);
}

void Add_Boolean_Constant(string _szName, bool _bBool)
{
	types::Bool* pVal = new types::Bool(_bBool);
	symbol::Context::getInstance()->put(_szName, *pVal);
}
