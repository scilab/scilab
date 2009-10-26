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
#include "windows_main.h"
#include "core_math.h"
#include "getcommandlineargs.h"
#include "scilabmode.h"
#include "forbiddenToUseScilab.h"
#include "realmain.h"
#include "version.h"
#include "PATH_MAX.h"
#include "getScilabDirectory.h"
#include "MALLOC.h"
#include "FilesAssociations.h"
#include "sciquit.h"
#include "scilab_main.h"
#include "stristr.h"
#include "console.h"
#include "WinConsole.h"
#include "StartupMessageBox.h"
#include "splashScreen.h"
#include "WndThread.h"
#include "strdup_windows.h"
#include "InnosetupMutex.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/ 
#define MIN_STACKSIZE 180000
#define WSCILEX "wscilex.exe"
/*--------------------------------------------------------------------------*/ 
static LPSTR my_argv[MAXCMDTOKENS];
static int my_argc = -1;
static int startupf = 0; /** 0 if we execute startup else 1 **/
static int  memory = MIN_STACKSIZE;
/*--------------------------------------------------------------------------*/ 
extern int sci_show_banner ; 
/*--------------------------------------------------------------------------*/ 
int Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
	BOOL ShortCircuitExec = FALSE;
	BOOL LaunchAFile = FALSE;
	char *ScilabDirectory = NULL;
	wchar_t *pwcFullCmdLineTmp = NULL;
	BOOL ShowMessageBoxInfo = TRUE;	
	LPWSTR *szArglist = NULL;
	char FileName[PATH_MAX * 2];
	int nArgs = 0;
	int i = 0;
	int  argcount = 0, lpath = 0;
	InitScriptType pathtype = SCILAB_SCRIPT;
	char *path = NULL;

	forbiddenToUseScilab();

	setScilabMode(SCILAB_STD);

	CreateScilabHiddenWndThread();

	ScilabDirectory = getScilabDirectory(FALSE);

	if (ScilabDirectory == NULL)
	{
		MessageBox (NULL,"ERROR" , "getScilabDirectory()", MB_ICONSTOP | MB_OK);
		exit(1);
	}
	else
	{
		FREE(ScilabDirectory);
		ScilabDirectory = NULL;
	}

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (szArglist)
	{
		for (i = 0; i < nArgs; i++)
		{
			my_argv[i] = wide_string_to_UTF8(szArglist[i]);
		}
		my_argc = nArgs;
		LocalFree(szArglist);
	}

	setCommandLineArgs(my_argv, my_argc);

	for (i=1;i<my_argc;i++)
	{
		if  ( _stricmp (my_argv[i], "-NB") == 0 )
		{
			ShowMessageBoxInfo = FALSE;
		}
	}

	// No more needed
	//if (ShowMessageBoxInfo) StartupMessageBox();

	for (i = 1; i < my_argc; i++)
	{
		if  ( (_stricmp (my_argv[i], "-NW") == 0) || (_stricmp (my_argv[i], "-NWI") == 0) || (_stricmp (my_argv[i], "-TEXMACS") == 0) || (_stricmp (my_argv[i], "-NOGUI") == 0) )
		{
			MessageBox(NULL,"Not with Windows Console","Error",MB_ICONINFORMATION);
			exit(1);
		}

		if ( (_stricmp (my_argv[i], "-VERSION") == 0) ||
			(_stricmp (my_argv[i], "-VER") == 0) )
		{
			disp_scilab_version();
			exit(1);
		}

		if ( (_stricmp (my_argv[i], "-H") == 0) ||
			(_stricmp (my_argv[i], "-?") == 0) ||
			(_stricmp (my_argv[i], "-HELP") == 0) )
		{
			char Msg[2048];
			strcpy(Msg,"Wscilex <Options> : run Scilab.\n");
			strcat(Msg,"Arguments : passes Arguments to Scilab, This Arguments can be retreived\n  by the Scilab function sciargs.\n"); 
			strcat(Msg,"-e Instruction : execute the scilab instruction given in Instruction argument.\n"); 
			strcat(Msg,"-f File : execute the scilab script given in File argument.\n"); 
			strcat(Msg,"  '-e' and '-f' options are mutually exclusive.\n\n"); 
			strcat(Msg,"-l lang : it fixes the user language.\n\n" ); 
			strcat(Msg,"-mem N : set the initial stacksize.\n"); 
			strcat(Msg,"-ns : if this option is present the startup file scilab.start is not executed.\n"); 
			strcat(Msg,"-nb : if this option is present then scilab welcome banner is not displayed.\n"); 
			strcat(Msg,"-nouserstartup : don't execute user startup files SCIHOME/.scilab or SCIHOME/scilab.ini.\n"); 
			strcat(Msg,"-nw : start Scilab without specialized Scilab Window.\n"); 
			strcat(Msg,"-nwni : start Scilab without user interaction (batch mode).\n"); 
			strcat(Msg,"-nogui : start Scilab without GUI,tcl/tk and user interaction (batch mode).\n"); 
			strcat(Msg,"-texmacs : reserved for WinTeXMacs.\n"); 
			strcat(Msg,"-version : print product version and exit.\n"); 

			MessageBox(NULL, Msg, "Help", MB_ICONINFORMATION);
			exit(1);
		}
	}

	argcount = my_argc;
	if (argcount > 2)
	{
		if ( (_stricmp (my_argv[1], "-X") == 0) ||
			(_stricmp (my_argv[1], "-O") == 0) ||	
			(_stricmp (my_argv[1], "-P") == 0) )
		{
			char *Commande = NULL;
			int CodeAction = -1;

			LaunchAFile = TRUE;
			strcpy(FileName, my_argv[2]);

			if (_stricmp (my_argv[1], "-O") == 0) CodeAction = 0;
			if (_stricmp (my_argv[1], "-X") == 0) CodeAction = 1; 
			if (_stricmp (my_argv[1], "-P") == 0) CodeAction = 2;

			Commande = (char*)MALLOC(((PATH_MAX*2)+1)*sizeof(char));
			strcpy(Commande, "empty");
			CommandByFileExtension(FileName, CodeAction, Commande);

			if (( ( IsAScicosFile(FileName)== TRUE ) && (CodeAction == 1) ) ||
				( ( IsAGraphFile(FileName)== TRUE  ) && (CodeAction == 1) )
				)
			{
				my_argc = -1;
				my_argv[++my_argc] = Commande;
				argcount = my_argc;
				ShortCircuitExec = TRUE;
			}
			else
			{
				my_argc = -1;
				my_argv[++my_argc] = strtok (Commande, " ");
				while (my_argv[my_argc] != NULL)
				{
					my_argv[++my_argc] = strtok(NULL, " ");
				}
				argcount = my_argc;
			}
		}
	}	

	if ( ShortCircuitExec == TRUE)
	{
		char PathWScilex[PATH_MAX*2];
		int lenPathWScilex = 0;
		GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), PathWScilex, PATH_MAX);
		lenPathWScilex = (int)strlen(PathWScilex);
		path = my_argv[argcount] + lenPathWScilex + 3;
		lpath = (int)strlen (my_argv[argcount] + lenPathWScilex + 3);
		pathtype = SCILAB_CODE;
		LaunchAFile = TRUE;
	}
	else while (argcount > 0)
	{
		char ArgTmp[PATH_MAX *2];

		argcount--;
		strcpy(ArgTmp, my_argv[argcount]);

		if (_stricmp (ArgTmp, "-NS") == 0) startupf = 1;
		else if ( _stricmp(ArgTmp,"-NB") == 0) { sci_show_banner = 0; }
		else if (_stricmp (ArgTmp, "-F") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = (int)strlen (my_argv[argcount + 1]);
		}
		else if (_stricmp (ArgTmp, "-E") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = (int)strlen (my_argv[argcount + 1]);
			pathtype = SCILAB_CODE;
		}
		else if ( _stricmp(ArgTmp,"-MEM") == 0 && argcount + 1 < my_argc)
		{
			memory = Max(atoi( my_argv[argcount + 1]),MIN_STACKSIZE );
		}
	}		

#ifndef _DEBUG
	if ( (sci_show_banner) && (LaunchAFile == FALSE) ) splashScreen();
#endif

	CreateScilabConsole(sci_show_banner);
	HideScilex(); /* hide console window */

	createInnosetupMutex();
	sci_windows_main (&startupf, path,(InitScriptType)pathtype, &lpath,memory);

	return 0;
}
/*--------------------------------------------------------------------------*/ 
