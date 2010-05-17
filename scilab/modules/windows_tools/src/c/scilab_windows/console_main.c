/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include <Windows.h>
#include <shellapi.h>
#include "console_main.h"
#include "core_math.h"
#include "getcommandlineargs.h"
#include "scilabmode.h"
#include "forbiddenToUseScilab.h"
#include "realmain.h"
#include "version.h"
#include "getScilabDirectory.h"
#include "MALLOC.h"
#include "sciquit.h"
#include "scilab_main.h"
#include "console.h"
#include "WndThread.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
#define MIN_STACKSIZE 180000
/*--------------------------------------------------------------------------*/ 
static LPSTR my_argv[MAXCMDTOKENS];
static int my_argc = -1;
static int startupf = 0; /** 0 if we execute startup else 1 **/
static int  memory = MIN_STACKSIZE;
/*--------------------------------------------------------------------------*/ 
extern void settexmacs(void);
/*--------------------------------------------------------------------------*/ 
int sci_show_banner = 1;
/*--------------------------------------------------------------------------*/ 
int Console_Main(int argc, char **argv)
{
  int nowin = 0, argcount = 0, lpath = 0;
  InitScriptType pathtype = SCILAB_SCRIPT;
  char *path = NULL;
  char *ScilabDirectory = NULL;
  int i=0;

  my_argc = -1;

  forbiddenToUseScilab();

  setScilabMode(SCILAB_NW);

  setCommandLineArgs(argv, argc);

  for (i=0;i<argc;i++)
  {
	  my_argv[i] = argv[i];
  }
  my_argc = argc;

  ScilabDirectory = getScilabDirectory(FALSE);

  if (ScilabDirectory == NULL)
  {
	MessageBox (NULL, "ERROR" ,"getScilabDirectory()", MB_ICONSTOP | MB_OK);
	exit(1);
  }
  else
  {
	FREE(ScilabDirectory);
	ScilabDirectory = NULL;
  }
 
  argcount = my_argc;
  while (argcount > 0)
  {
	argcount--;
    if (_stricmp (my_argv[argcount], "-NW") == 0) nowin = 1;
    else if (_stricmp (my_argv[argcount], "-NS") == 0) startupf = 1;
    else if ( _stricmp(my_argv[argcount],"-NB") == 0) { sci_show_banner = 0; }
    else if (_stricmp (my_argv[argcount], "-NWNI") == 0)
	{
		setScilabMode(SCILAB_NWNI);
	}
    else if (_stricmp (my_argv[argcount], "-F") == 0 && argcount + 1 < my_argc)
	{
		path = my_argv[argcount + 1];
		lpath = (int) strlen (my_argv[argcount + 1]);
	}
    else if (_stricmp (my_argv[argcount], "-E") == 0 && argcount + 1 < my_argc)
	{
		path = my_argv[argcount + 1];
		lpath = (int) strlen (my_argv[argcount + 1]);
		pathtype = SCILAB_CODE;
	}
    else if ( _stricmp(my_argv[argcount],"-MEM") == 0 && argcount + 1 < my_argc)
	{
		memory = Max(atoi( my_argv[argcount + 1]),MIN_STACKSIZE );
	} 
	else if ( _stricmp(my_argv[argcount],"-TEXMACS") == 0 )
	{
	  setScilabMode(SCILAB_NWNI);
	  settexmacs();
	}
	else if ( _stricmp(my_argv[argcount],"-NOGUI") == 0 )
	{
	  setScilabMode(SCILAB_NWNI);
	}
	else if ( (_stricmp (my_argv[argcount],"-VERSION")==0) ||
	  (_stricmp (my_argv[argcount],"-VER")==0) )
	{
		disp_scilab_version();
		exit(1);
	}
	else if ( (_stricmp (my_argv[argcount],"-H")==0) ||
	  (_stricmp (my_argv[argcount],"-?")==0) ||
	  (_stricmp (my_argv[argcount],"-HELP")==0) )
	{
		printf("scilex <Options> : run Scilab.\n");
		printf("Arguments : passes Arguments to Scilab, This Arguments can be retreived\n  by the Scilab function sciargs.\n"); 
		printf("-e Instruction : execute the scilab instruction given in Instruction argument.\n"); 
		printf("-f File : execute the scilab script given in File argument.\n"); 
		printf("  '-e' and '-f' options are mutually exclusive.\n\n"); 
		printf("-l lang : it fixes the user language.\n\n" ); 
		printf("-mem N : set the initial stacksize.\n"); 
		printf("-ns : if this option is present the startup file scilab.start is not executed.\n"); 
		printf("-nb : if this option is present then scilab welcome banner is not displayed.\n"); 
		printf("-nouserstartup : don't execute user startup files SCIHOME/.scilab or SCIHOME/scilab.ini.\n"); 
		printf("-nw : start Scilab without specialized Scilab Window.\n"); 
		printf("-nwni : start Scilab without user interaction (batch mode).\n"); 
		printf("-nogui : start Scilab without GUI,tcl/tk and user interaction (batch mode).\n"); 
		printf("-texmacs : reserved for WinTeXMacs.\n"); 
		printf("-version : print product version and exit.\n"); 
		printf("\n");
		exit(1);
	}
  }

  if (getScilabMode() != SCILAB_NWNI) CreateScilabHiddenWndThread();

  if ( (getScilabMode() == SCILAB_NWNI) || (getScilabMode() == SCILAB_NW) )
    {
	  SaveConsoleColors();
	  if (getScilabMode() == SCILAB_NW)
	  {
		  RenameConsole();
		  UpdateConsoleColors();
	  }
	  
	  sci_windows_main (&startupf, path, (InitScriptType)pathtype, &lpath,memory);
	 
    }
  else
    {
        MessageBox(NULL,"-nw or -nwni not found","ERROR",MB_ICONWARNING);
    }
  return 0;
}
/*--------------------------------------------------------------------------*/ 
