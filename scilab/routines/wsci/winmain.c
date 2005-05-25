/*******************************************
* Original source : GNUPLOT - winmain.c
* modified for Scilab
*******************************************
* Copyright (C) 1992   Maurice Castro, Russell Lang
*
* Permission to use, copy, and distribute this software and its
* documentation for any purpose with or without fee is hereby granted,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.
*
* Permission to modify the software is granted, but not the right to
* distribute the modified code.  Modifications are to be distributed
* as patches to released version.
*
* This software is provided "as is" without express or implied warranty.
*
*
* AUTHORS
*
*   Maurice Castro
*   Russell Lang
*   Modified for Scilab (1997) : Jean-Philippe Chancelier
*   Modified for Scilab (2003) : Allan CORNET
*
*/

#ifndef STRICT
#define STRICT
#endif

#include "winmain.h"
#include "WinConsole.h"

BOOL ScilabIsStarting=TRUE;
int  sci_show_banner=1;

/*-----------------------------------------------------------------------------------*/
int MAIN__ ()
{
	#ifndef __ABSC__
	HANDLE x = GetModuleHandleA (0);
	Windows_Main  (x, 0, GetCommandLineA (), 1);
	#else
	HMODULE x = GetModuleHandle (0);
	Windows_Main (x, 0, GetCommandLine (), 1);
	#endif
	return (0);
}
/*-----------------------------------------------------------------------------------*/
int Console_Main(int argc, char **argv)
{
  LPSTR tail;
  int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
  char *path = NULL;
  
  int i=0;
  my_argc = -1;

  ForbiddenToUseScilab();

  SetWindowMode(FALSE);
  
  for (i=0;i<argc;i++)
  {
	  my_argv[i] = argv[i];
  }
  my_argc =argc;

  szModuleName = (LPSTR) malloc (MAXSTR + 1);
  CheckMemory (szModuleName);

  /* get path to EXE */
  GetModuleFileName (GetModuleHandle (0), (LPSTR) szModuleName, MAXSTR);
  if (CheckWScilabVersion (WSCILABVERSION))
    {
      MessageBox (NULL, "Wrong version of WSCILAB.DLL", szModuleName, MB_ICONSTOP | MB_OK);
      exit (1);
    }

  if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
    {
      tail++;
      *tail = '\0';
    }
  szModuleName = realloc (szModuleName, strlen (szModuleName) + 1);
  CheckMemory (szModuleName);

  szMenuName = (LPSTR) malloc (strlen (szModuleName) + strlen (MENUNAME) + 1);
  CheckMemory (szMenuName);
  strcpy (szMenuName, szModuleName);
  strcat (szMenuName, MENUNAME);

  szGraphMenuName = (LPSTR) malloc (strlen (szModuleName) + strlen (GRAPHMENUNAME) + 1);
  CheckMemory (szGraphMenuName);
  strcpy (szGraphMenuName, szModuleName);
  strcat (szGraphMenuName, GRAPHMENUNAME);

  /* Load common control library * */
  InitCommonControls ();

  textwin.hInstance = GetModuleHandle("LibScilab");
  textwin.hPrevInstance = 0;
  textwin.nCmdShow = 1;
  textwin.Title = "Scilab";
  textwin.IniFile = "scilab.ini";
  textwin.IniSection = "SCILAB";
  textwin.DragPre = "gl_name='";
  textwin.DragPost = "';exec('SCI/util/GLoad.sce');\n";
  textwin.lpmw = &menuwin;
  textwin.ScreenSize.x = 120;
  textwin.ScreenSize.y = 80;
  textwin.KeyBufSize = 2048;
  textwin.CursorFlag = 1;	/* scroll to cursor after \n & \r */
  textwin.shutdown = (DLGPROC) ShutDown;
  textwin.AboutText = (LPSTR) malloc (1024);
  CheckMemory (textwin.AboutText);
  strcpy (textwin.AboutText, "Scilab is a free copyrighted software.\n\t");
  strcat (textwin.AboutText, "developed by Scilab Group (Inria/Enpc)\n\t");
  strcat (textwin.AboutText, "\n\t email: scilab@inria.fr\n\t");
  textwin.AboutText = realloc (textwin.AboutText, strlen (textwin.AboutText) + 1);
  CheckMemory (textwin.AboutText);

  menuwin.szMenuName = szMenuName;

  pausewin.hInstance = GetModuleHandle (0);
  pausewin.hPrevInstance = 0;
  pausewin.Title = "Scilab pause";

  graphwin.hInstance = GetModuleHandle (0);
  graphwin.hPrevInstance = 0;
  graphwin.Title = "Scilab Graph";
  graphwin.szMenuName = szGraphMenuName;
  graphwin.lptw = &textwin;
  graphwin.IniFile = textwin.IniFile;
  graphwin.IniSection = textwin.IniSection;
  argcount = my_argc;
  while (argcount > 0)
    {
      argcount--;
      if (strcmp (my_argv[argcount], "-nw") == 0) nowin = 1;
      else if (strcmp (my_argv[argcount], "-ns") == 0) startupf = 1;
      else if ( strcmp(my_argv[argcount],"-nb") == 0) { sci_show_banner = 0; }
      else if (strcmp (my_argv[argcount], "-nwni") == 0)
		{
			nowin = 1;
			nointeractive = 1;
		}
      else if (strcmp (my_argv[argcount], "-f") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = strlen (my_argv[argcount + 1]);
		}
      else if (strcmp (my_argv[argcount], "-e") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = strlen (my_argv[argcount + 1]);
			pathtype=1;
		}
      else if ( strcmp(my_argv[argcount],"-mem") == 0 && argcount + 1 < my_argc)
		{
			memory = Max(atoi( my_argv[argcount + 1]),MIN_STACKSIZE );} 
		}
#ifndef __DLL__
  /** when we don't use a dll version of the graphic library 
    which is the case up to now **/
  NoDll (GetModuleHandle (0));
#endif
  atexit (WinExit);
  SciEnv ();
  if (nowin == 1)
    {
	  SaveConsoleColors();
	  if (nointeractive!=1)
	  {
		  RenameConsole();
		  UpdateConsoleColors();
	  }

	  start_sci_gtk() ;
	  sci_windows_main (nowin, &startupf,path,pathtype, &lpath,memory);
	 
    }
  else
    {
        MessageBox(NULL,"-nw or -nwni not found","Error",MB_ICONWARNING);
    }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int WINAPI Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
	int i=0;
	LPSTR tail;
	BOOL ShortCircuitExec=FALSE;
	BOOL LaunchAFile=FALSE;
	char FileName[MAX_PATH];
	int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
	char *path = NULL;
	
	HANDLE hOut = NULL;

	char *pFullCmdLine=NULL;
	char *pFullCmdLineTmp=NULL;
	char *pPathCmdLine=NULL;

	ForbiddenToUseScilab();

	SetWindowMode(TRUE);
	ScilabIsStarting=TRUE;

	strcpy(FileName,"Empty");

	/* New Graphics Mode Warning */
	MessageBoxNewGraphicMode();
	
	szModuleName = (LPSTR) malloc (MAXSTR + 1);
	CheckMemory (szModuleName);

	/* get path to EXE */
	GetModuleFileName (hInstance, (LPSTR) szModuleName, MAXSTR);
	if (CheckWScilabVersion (WSCILABVERSION))
	{
		MessageBox (NULL, "Wrong version of libScilab.DLL", szModuleName, MB_ICONSTOP | MB_OK);
		exit (1);
	}

	if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
	{
		tail++;
		*tail = '\0';
	}
	szModuleName = realloc (szModuleName, strlen (szModuleName) + 1);
	CheckMemory (szModuleName);

	szMenuName = (LPSTR) malloc (strlen (szModuleName) + strlen (MENUNAME) + 1);
	CheckMemory (szMenuName);
	strcpy (szMenuName, szModuleName);
	strcat (szMenuName, MENUNAME);

	szGraphMenuName = (LPSTR) malloc (strlen (szModuleName) + strlen (GRAPHMENUNAME) + 1);
	CheckMemory (szGraphMenuName);
	strcpy (szGraphMenuName, szModuleName);
	strcat (szGraphMenuName, GRAPHMENUNAME);

	/* Load common control library * */
	InitCommonControls ();

	textwin.hInstance = hInstance;
	textwin.hPrevInstance = hPrevInstance;
	textwin.nCmdShow = iCmdShow;
	textwin.Title = "Scilab";
	textwin.IniFile = "scilab.ini";
	textwin.IniSection = "SCILAB";
	textwin.DragPre = "gl_name='";
	textwin.DragPost = "';exec('SCI/util/GLoad.sce');\n";
	textwin.lpmw = &menuwin;
	textwin.ScreenSize.x = 120;
	textwin.ScreenSize.y = 80;
	textwin.KeyBufSize = 4096;
	textwin.CursorFlag = 1;	/* scroll to cursor after \n & \r */
	textwin.shutdown = (DLGPROC) ShutDown;
	textwin.AboutText = (LPSTR) malloc (1024);
	CheckMemory (textwin.AboutText);
	strcpy (textwin.AboutText, "Scilab is a free copyrighted software.\n\t");
	strcat (textwin.AboutText, "developed by Scilab Consortium (Inria/Enpc)\n\t");
	strcat (textwin.AboutText, "\n\t email: scilab@inria.fr\n\t");
	textwin.AboutText = realloc (textwin.AboutText, strlen (textwin.AboutText) + 1);
	CheckMemory (textwin.AboutText);

	menuwin.szMenuName = szMenuName;

	pausewin.hInstance = hInstance;
	pausewin.hPrevInstance = hPrevInstance;
	pausewin.Title = "Scilab pause";

	graphwin.hInstance = hInstance;
	graphwin.hPrevInstance = hPrevInstance;
	graphwin.Title = "Scilab Graph";
	graphwin.szMenuName = szGraphMenuName;
	graphwin.lptw = &textwin;
	graphwin.IniFile = textwin.IniFile;
	graphwin.IniSection = textwin.IniSection;

	pFullCmdLineTmp=GetCommandLine();
	{
		char LineCommand[MAX_PATH];
		char LineCommandBis[MAX_PATH];

		char ShortPath[MAX_PATH];
		char *pPathCmdLine=NULL;
		char PathCmdLineCopy[1024];

		strcpy(LineCommand,pFullCmdLineTmp);
		LineCommand[strlen(LineCommand)]='\0';
		strcpy(LineCommandBis,pFullCmdLineTmp);
		LineCommandBis[strlen(LineCommandBis)]='\0';

		pPathCmdLine=strstr(LineCommand,"wscilex.exe\" ");
		
		if ( (pPathCmdLine != NULL) && ( strlen(pPathCmdLine)-strlen("wscilex.exe\" ")-1 > 0) ) 
		{
			char LINE[1024];

			strcpy(PathCmdLineCopy,pPathCmdLine);	
			if ( PathCmdLineCopy[strlen("wscilex.exe\" ")-2] == '\"' ) PathCmdLineCopy[strlen("wscilex.exe\" ")-2] = ' ';
			strncpy(LINE,&LineCommandBis[1],strlen(LineCommandBis)-strlen(PathCmdLineCopy)-1);
			LINE[strlen(LineCommandBis)-strlen(PathCmdLineCopy)-1]='\0';
		
			GetShortPathName(LINE,ShortPath,MAX_PATH);
			strcat(ShortPath,PathCmdLineCopy);
		
			pFullCmdLine=(char*)malloc(sizeof(char)*(strlen(ShortPath)+1));
			strcpy(pFullCmdLine,ShortPath);
		}
		else
		{
			pFullCmdLine=(char*)malloc(sizeof(char)*(strlen(LineCommandBis)+1));
			strcpy(pFullCmdLine,LineCommandBis);
		}
		
	}
	
	my_argv[++my_argc] = strtok (pFullCmdLine, " ");
	while (my_argv[my_argc] != NULL)
	{
		my_argv[++my_argc] = strtok(NULL, " ");
	}

	for (i=1;i<my_argc;i++)
	{
		char ArgTmp[MAX_PATH];
		strcpy(ArgTmp,my_argv[i]);
		if  ( (strcmp (strupr(ArgTmp), "-NW") == 0) || (strcmp (strupr(ArgTmp), "-NWI") == 0) )
		{
			MessageBox(NULL,"Not with Windows Console","Error",MB_ICONINFORMATION);
			exit(1);
		}

		if ( (strcmp (strupr(ArgTmp), "-H") == 0) ||
			 (strcmp (strupr(ArgTmp), "-?") == 0) ||
			 (strcmp (strupr(ArgTmp), "-HELP") == 0) )
		{
			char Msg[1024];
			strcpy(Msg,"Wscilex <Options> : run Scilab.\n");
			strcat(Msg,"-ns:if this option is present the startup file SCI/scilab.star is not executed.\n"); 
			strcat(Msg,"-nb:if this option is present then the scilab welcome banner is not displayed.\n"); 
			strcat(Msg,"-l lang: it fixes the user language.\nThe possible lang values are 'fr' for french and 'eng' for english.\nThe default language is english. This default value is fixed the scilab.star file.\n"); 
			strcat(Msg,"-f file: Scilab script file is executed first into Scilab.\n"); 
			strcat(Msg,"-e instruction: Scilab instruction instruction is executed first into Scilab.");
			MessageBox(NULL,Msg,"Help",MB_ICONINFORMATION);
			exit(1);
		}
	}
	argcount = my_argc;

	if (argcount > 2)
	{
		if ( (strcmp (my_argv[1], "-X") == 0) ||
		     (strcmp (my_argv[1], "-O") == 0) ||	
		     (strcmp (my_argv[1], "-P") == 0) )
		{
			char *Commande=NULL;
			int CodeAction=-1;
			int j=0;

			LaunchAFile=TRUE;

			strcpy(FileName,my_argv[2]);
			for (j=3;j<argcount;j++)
			{
				strcat(FileName," ");
				strcat(FileName,my_argv[j]);
			}
			if (strcmp (my_argv[1], "-O") == 0) CodeAction=0;
			if (strcmp (my_argv[1], "-X") == 0) CodeAction=1; 
			if (strcmp (my_argv[1], "-P") == 0) CodeAction=2;

			Commande=(char*)malloc(MAX_PATH*sizeof(char));
			strcpy(Commande,"empty");
			CommandByFileExtension(FileName,CodeAction,Commande);

			if (
				( ( IsAScicosFile(FileName)== TRUE ) && (CodeAction==1) ) ||
				( ( IsAGraphFile(FileName)== TRUE  ) && (CodeAction==1) )
			    )
			{
				my_argc=-1;
				my_argv[++my_argc]=Commande;
				argcount = my_argc;
				ShortCircuitExec=TRUE;
			}
			else
			{
				my_argc=-1;
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
		char PathWScilex[MAX_PATH];
		int lenPathWScilex=0;
		GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), PathWScilex, MAX_PATH);
		lenPathWScilex=strlen(PathWScilex);
		path = my_argv[argcount]+lenPathWScilex+3;
		lpath = strlen (my_argv[argcount]+lenPathWScilex+3);
		pathtype=1;
		LaunchAFile=TRUE;
	}
	else
	while (argcount > 0)
	{
        char ArgTmp[MAX_PATH];
                
		argcount--;
		strcpy(ArgTmp,my_argv[argcount]);
		
		if (strcmp (strupr(ArgTmp), "-NS") == 0) startupf = 1;
		else if ( strcmp(strupr(ArgTmp),"-NB") == 0) { sci_show_banner = 0; }
		else if (strcmp (strupr(ArgTmp), "-F") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = strlen (my_argv[argcount + 1]);
		}
		else if (strcmp (strupr(ArgTmp), "-E") == 0 && argcount + 1 < my_argc)
		{
			path = my_argv[argcount + 1];
			lpath = strlen (my_argv[argcount + 1]);
			pathtype=1;
		}
		else if ( strcmp(strupr(ArgTmp),"-MEM") == 0 && argcount + 1 < my_argc)
		{
			memory = Max(atoi( my_argv[argcount + 1]),MIN_STACKSIZE );
		}
	}		

	
	#ifndef __DLL__
		/** when we don't use a dll version of the graphic library
		which is the case up to now **/
		NoDll (hInstance);
	#endif
	atexit (WinExit);
	SciEnv ();

	/* Splashscreen*/
	if ( (sci_show_banner) && (LaunchAFile == FALSE) )CreateSplashscreen();

	CreateScilabConsole(sci_show_banner);

	if (TextInit (&textwin))	exit (1);

	textwin.hIcon = LoadIcon (hInstance, "texticon");
	SetClassLong (textwin.hWndParent, GCL_HICON, (DWORD) textwin.hIcon);
	SetXsciOn ();

	ShowWindow (textwin.hWndParent, SW_SHOWNORMAL);
	ForceToActiveWindowParent();
	HideScilex(); /* Cache la fenetre Console */

	if (LaunchAFile) ChangeCursorWhenScilabIsReady();

	sci_windows_main (nowin, &startupf, path,pathtype, &lpath,memory);

	CloseScilabConsole();
	/* Tue ce process pour fermeture correcte sous Windows 98 */
	Kill_Scilex_Win98();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
void InitWindowGraphDll(void)
/* Modification Allan CORNET*/
/* November 2003 */
/* Display graphic menus with a call of the DLL Scilab*/
/* for Interface with Java */
{
  
  HINSTANCE hdllInstanceTmp=NULL;
  char *p1 = (char*)getenv ("SCI");
  hdllInstanceTmp=(HINSTANCE)GetModuleHandle("libScilab");

  if (hdllInstanceTmp==NULL) 
  {
  	MessageBox(NULL,"Do not find LibScilab.dll","Error",MB_ICONWARNING|MB_OK);
  	exit(1);
  }

  ForbiddenToUseScilab();

  hdllInstance=hdllInstanceTmp;
  szModuleName = (LPSTR) malloc (MAXSTR + 1);
  CheckMemory (szModuleName);

  strcpy(szModuleName,p1);
  strcat(szModuleName,"\\bin\\");

  if (CheckWScilabVersion (WSCILABVERSION))
    {
      MessageBox (NULL, "Wrong version of LibScilab.dll", szModuleName, MB_ICONSTOP | MB_OK);
      exit (1);
    }

   szGraphMenuName = (LPSTR) malloc (strlen (szModuleName) + strlen (GRAPHMENUNAME) + 1);
   CheckMemory (szGraphMenuName);
   strcpy (szGraphMenuName, szModuleName);
   strcat (szGraphMenuName, GRAPHMENUNAME);
    
   InitCommonControls ();
  
   graphwin.hInstance = hdllInstance;
   graphwin.hPrevInstance = NULL;
   graphwin.Title = "Scilab Graph";
   graphwin.szMenuName = szGraphMenuName;
   graphwin.lptw = &textwin;
   graphwin.IniFile = "scilab.ini";
   graphwin.IniSection = "SCILAB";
   sci_tk_activate();
    
}
/*-----------------------------------------------------------------------------------*/
/* to simulate argv */
void add_sci_argv(char *p)
{
	if (*p)	my_argv[++my_argc]=p;
}
/*-----------------------------------------------------------------------------------*/
/* Fortran iargc and fgetarg implemented here */
int sci_iargc()
{
	return my_argc -1 ;
}
/*-----------------------------------------------------------------------------------*/
int sci_getarg(int *n,char *s,long int ls)
{
	register char *t;
	register int i;
	
	if(*n>=0 && *n <= my_argc)
	t = my_argv[*n];
	else
	t = "";

	for(i = 0; i < ls && *t!='\0' ; ++i)
	*s++ = *t++;
	for( ; i<ls ; ++i)
	*s++ = ' ';
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int InteractiveMode ()
{
	return nointeractive;
}
/*-----------------------------------------------------------------------------------*/
int C2F(showlogo) ()
{
	return show_logo;
}
/*-----------------------------------------------------------------------------------*/
static void CheckMemory (LPSTR str)
{
#ifndef _DEBUG
	if (str == (LPSTR) NULL)
	{
		MessageBox (NULL, "out of memory", "Scilab", MB_ICONSTOP | MB_OK);
		exit (1);
	}
#endif
}
/*-----------------------------------------------------------------------------------*/
int Pause (LPSTR str)
{
	pausewin.Message = str;
	return (PauseBox (&pausewin) == IDOK);
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* atexit procedure for scilab and scilab -nw
*---------------------------------------------------*/

void WinExit (void)
{
	int i;
	C2F (xscion) (&i);
	if (i != 0)
	{
		/** delete all graph windows **/
		AllGraphWinDelete ();
		TextMessage ();		/* process messages */
		TextClose (&textwin);
		TextMessage ();		/* process messages */
	}
	else
	{
	  RestoreConsoleColors();
	}
	return;
}
/*-----------------------------------------------------------------------------------*/
/* utility function for WinExit */
/*-----------------------------------------------------------------------------------*/
static void AllGraphWinDelete ()
{
	integer iflag = 0, num, *ids = (integer *) 0;
	C2F (getwins) (&num, ids, &iflag);
	ids = malloc ((unsigned) num * sizeof (integer));
	iflag = 1;
	if (ids != NULL)
	{
		int i;
		C2F (getwins) (&num, ids, &iflag);
		for (i = 0; i < num; i++)
		C2F (deletewin) (&ids[i]);
		free (ids);
	}
}
/*-----------------------------------------------------------------------------------*/
/* call back function from Text Window WM_CLOSE */

EXPORT int CALLBACK  ShutDown (void)
{
	exit (0);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Modification Correction Bug Win 9x Winoldap */
/* Allan CORNET le 16/07/03 */
void Kill_Scilex_Win98(void)
{
	/* Detection de la version de Windows */
	if ( GetVersion() < 0x80000000 )
	{
		/* Windows NT */
		/* Sortie Normale */

	}
	else
	{
		/* Win32s, Win95,Win98,WinME */
		Kill_Scilex();
	}
}
/*-----------------------------------------------------------------------------------*/
void Kill_Scilex(void)
{
	HANDLE hProcess;
	/* Ouverture de ce Process avec droit pour le tuer */

	hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,GetCurrentProcessId());
	if (hProcess)
	{
		/* Tue ce Process */

		TerminateProcess(hProcess,0);
	}
	else MessageBox(NULL,"Don't Find Scilab Process","Attention",MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void CreateSplashscreen(void)
{
	HWND hdlg;
	char buffer[MAX_PATH];

	wsprintf(buffer,"%s %s","Copyright ® ",DEFAULT_MES);

	hdlg = CreateDialog(hdllInstance, "IDD_SPLASH", NULL,NULL);
	SetDlgItemText(hdlg,IDC_VERSION_SPLASH,VERSION);
	SetDlgItemText(hdlg,IDC_COPYRIGHT_SPLASH,buffer);
	
	ShowWindow(hdlg, SW_SHOWNORMAL);
	UpdateWindow(hdlg);
	Sleep(1500);

	DestroyWindow(hdlg);
}
/*-----------------------------------------------------------------------------------*/
BOOL ForbiddenToUseScilab(void)
{
	BOOL bOK=FALSE;
	int WinVer=GetOSVersion();
	HDC hdc=GetDC(NULL);
	int BitsByPixel = GetDeviceCaps(hdc, BITSPIXEL);

    ReleaseDC (NULL, hdc);

	if ( (WinVer == OS_WIN32_WINDOWS_NT_3_51) || (WinVer == OS_WIN32_WINDOWS_NT_4_0) )
	{
		MessageBox(NULL,"Warning","Scilab doesn''t support Windows NT 3.51 or 4.\n",MB_ICONSTOP);
		exit(1);
		return bOK;
	}

	if ( BitsByPixel < 8 )
	{
		MessageBox(NULL,"Warning","Scilab supports only 256 colors or more.\n",MB_ICONSTOP);
		exit(1);
		return bOK;
	}

	bOK=TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
