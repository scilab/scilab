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

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "win_mem_alloc.h" /* MALLOC */

BOOL ScilabIsStarting=TRUE;
int  sci_show_banner=1;

/*-----------------------------------------------------------------------------------*/
int MAIN__ ()
{
	HANDLE x = GetModuleHandleA (0);
	Windows_Main  (x, 0, GetCommandLineA (), 1);
	return (0);
}
/*-----------------------------------------------------------------------------------*/
int Console_Main(int argc, char **argv)
{
  int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
  char *path = NULL;
  char *ScilabDirectory=NULL;
  int i=0;

  my_argc = -1;

  ForbiddenToUseScilab();

  SetWindowMode(FALSE);
  
  for (i=0;i<argc;i++)
  {
	  my_argv[i] = argv[i];
  }
  my_argc =argc;

  ScilabDirectory=GetScilabDirectory(FALSE);

  if (ScilabDirectory == NULL)
  {
	MessageBox (NULL, MSG_ERROR20 ,MSG_ERROR38, MB_ICONSTOP | MB_OK);
	exit(1);
  }

  szMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (MENUNAME) + 1);
  CheckMemory (szMenuName);
  strcpy (szMenuName, ScilabDirectory);
  strcat (szMenuName, MENUNAME);

  szGraphMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (GRAPHMENUNAME) + 1);
  CheckMemory (szGraphMenuName);
  strcpy (szGraphMenuName, ScilabDirectory);
  strcat (szGraphMenuName, GRAPHMENUNAME);

  if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}

  /* Load common control library * */
  InitCommonControls ();

  textwin.hInstance = GetModuleHandle(MSG_SCIMSG9);
  textwin.hPrevInstance = 0;
  textwin.nCmdShow = 1;
  textwin.Title = MSG_SCIMSG21;
  textwin.IniFile = "scilab.ini";
  textwin.IniSection = "SCILAB";
  textwin.DragPre = "gl_name='";
  textwin.DragPost = MSG_SCIMSG17;
  textwin.lpmw = &menuwin;
  textwin.ScreenSize.x = 120;
  textwin.ScreenSize.y = 80;
  textwin.KeyBufSize = 2048;
  textwin.CursorFlag = 1;	/* scroll to cursor after \n & \r */
  textwin.shutdown = (DLGPROC) ShutDown;
  textwin.AboutText = (LPSTR) MALLOC (1024);
  CheckMemory (textwin.AboutText);
  strcpy (textwin.AboutText, MSG_SCIMSG18);
  strcat (textwin.AboutText,MSG_SCIMSG19);
  strcat (textwin.AboutText,MSG_SCIMSG20);
  textwin.AboutText = REALLOC (textwin.AboutText, strlen (textwin.AboutText) + 1);
  CheckMemory (textwin.AboutText);

  menuwin.szMenuName = szMenuName;

  pausewin.hInstance = GetModuleHandle(MSG_SCIMSG9);
  pausewin.hPrevInstance = 0;
  pausewin.Title = MSG_SCIMSG22;

  graphwin.hInstance = GetModuleHandle(MSG_SCIMSG9);
  graphwin.hPrevInstance = 0;
  graphwin.Title =  MSG_SCIMSG23 ;
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
        MessageBox(NULL,MSG_ERROR79,MSG_ERROR20,MB_ICONWARNING);
    }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int WINAPI Windows_Main (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
	int i=0;
	BOOL ShortCircuitExec=FALSE;
	BOOL LaunchAFile=FALSE;
	char FileName[MAX_PATH];
	int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
	char *path = NULL;
	char *ScilabDirectory=NULL;

	HANDLE hOut = NULL;
	
	char *pFullCmdLine=NULL;
	char *pFullCmdLineTmp=NULL;
	char *pPathCmdLine=NULL;

	ForbiddenToUseScilab();

	SetWindowMode(TRUE);
	ScilabIsStarting=TRUE;

	strcpy(FileName,"Empty");

	/* New Graphics Mode Warning */
	/*MessageBoxNewGraphicMode();*/

	ScilabDirectory=GetScilabDirectory(FALSE);

	if (ScilabDirectory == NULL)
	{
		MessageBox (NULL,MSG_ERROR20 , MSG_ERROR38, MB_ICONSTOP | MB_OK);
		exit(1);
	}	
	
	szMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (MENUNAME) + 1);
	CheckMemory (szMenuName);
	strcpy (szMenuName, ScilabDirectory);
	strcat (szMenuName, MENUNAME);

	szGraphMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen (GRAPHMENUNAME) + 1);
	CheckMemory (szGraphMenuName);
	strcpy (szGraphMenuName, ScilabDirectory);
	strcat (szGraphMenuName, GRAPHMENUNAME);

	if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}

	/* Load common control library * */
	InitCommonControls ();

	textwin.hInstance = hInstance;
	textwin.hPrevInstance = hPrevInstance;
	textwin.nCmdShow = iCmdShow;
	textwin.Title = MSG_SCIMSG21;
	textwin.IniFile = "scilab.ini";
	textwin.IniSection = "SCILAB";
	textwin.DragPre = "gl_name='";
	textwin.DragPost = MSG_SCIMSG17;
	textwin.lpmw = &menuwin;
	textwin.ScreenSize.x = 120;
	textwin.ScreenSize.y = 80;
	textwin.KeyBufSize = 4096;
	textwin.CursorFlag = 1;	/* scroll to cursor after \n & \r */
	textwin.shutdown = (DLGPROC) ShutDown;
	textwin.AboutText = (LPSTR) MALLOC (1024);
	CheckMemory (textwin.AboutText);
	strcpy (textwin.AboutText,MSG_SCIMSG18);
	strcat (textwin.AboutText, MSG_SCIMSG19);
	strcat (textwin.AboutText, MSG_SCIMSG20);
	textwin.AboutText = REALLOC (textwin.AboutText, strlen (textwin.AboutText) + 1);
	CheckMemory (textwin.AboutText);

	menuwin.szMenuName = szMenuName;

	pausewin.hInstance = hInstance;
	pausewin.hPrevInstance = hPrevInstance;
	pausewin.Title = MSG_SCIMSG22;

	graphwin.hInstance = hInstance;
	graphwin.hPrevInstance = hPrevInstance;
	graphwin.Title = MSG_SCIMSG23;
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
		
			pFullCmdLine=(char*)MALLOC(sizeof(char)*(strlen(ShortPath)+1));
			strcpy(pFullCmdLine,ShortPath);
		}
		else
		{
			pFullCmdLine=(char*)MALLOC(sizeof(char)*(strlen(LineCommandBis)+1));
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
			strcpy(Msg,MSG_SCIMSG24);
			strcat(Msg,MSG_SCIMSG25); 
			strcat(Msg,MSG_SCIMSG26); 
			strcat(Msg,MSG_SCIMSG27); 
			strcat(Msg,MSG_SCIMSG28); 
			strcat(Msg,MSG_SCIMSG29);
			MessageBox(NULL,Msg,MSG_SCIMSG30,MB_ICONINFORMATION);
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

			Commande=(char*)MALLOC(MAX_PATH*sizeof(char));
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
	SciEnv();

	/* Splashscreen*/
	if ( (sci_show_banner) && (LaunchAFile == FALSE) )CreateSplashscreen();

	CreateScilabConsole(sci_show_banner);

	if (TextInit (&textwin)) exit (1);

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
  char *ScilabDirectory=NULL;

  HINSTANCE hdllInstanceTmp=NULL;
  char *p1 = (char*)getenv ("SCI");
  hdllInstanceTmp=(HINSTANCE)GetModuleHandle(MSG_SCIMSG11);

  if (hdllInstanceTmp==NULL) 
  {
  	MessageBox(NULL,MSG_ERROR39,MSG_ERROR20,MB_ICONWARNING|MB_OK);
  	exit(1);
  }

  ForbiddenToUseScilab();
  
  hdllInstance=hdllInstanceTmp;
  ScilabDirectory=GetScilabDirectory(FALSE);

  if (ScilabDirectory == NULL)
  {
	MessageBox (NULL, MSG_ERROR20, MSG_ERROR38, MB_ICONSTOP | MB_OK);
	exit(1);
  }	

  szGraphMenuName = (LPSTR) MALLOC (strlen (ScilabDirectory) + strlen("\\bin\\")+strlen (GRAPHMENUNAME) + 1);
  CheckMemory (szGraphMenuName);
  wsprintf(szGraphMenuName,"%s\\bin\\%s",ScilabDirectory,GRAPHMENUNAME);
  if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}		
  
   InitCommonControls ();
  
   graphwin.hInstance = hdllInstance;
   graphwin.hPrevInstance = NULL;
   graphwin.Title = MSG_SCIMSG23;
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
		MessageBox (NULL,MSG_ERROR40, MSG_ERROR20, MB_ICONSTOP | MB_OK);
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
	if (num > 0)
	{
		ids = MALLOC ((unsigned) num * sizeof (integer));
	}
	iflag = 1;

	if (ids != NULL)
	{
		int i;
		C2F (getwins) (&num, ids, &iflag);
		for (i = 0; i < num; i++)
		C2F (deletewin) (&ids[i]);
		FREE (ids);
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
	else MessageBox(NULL,MSG_WARNING20,MSG_WARNING21,MB_ICONWARNING);
}
/*-----------------------------------------------------------------------------------*/
void CreateSplashscreen(void)
{
	HWND hdlg;
	char buffer[MAX_PATH];

	wsprintf(buffer,"%s %s",MSG_SCIMSG31,DEFAULT_MES);

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
		MessageBox(NULL,MSG_WARNING22,MSG_ERROR41,MB_ICONSTOP);
		exit(1);
		return bOK;
	}

	if ( BitsByPixel < 8 )
	{
		MessageBox(NULL,MSG_WARNING22,MSG_ERROR42,MB_ICONSTOP);
		exit(1);
		return bOK;
	}

	bOK=TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
