#define NameConsole "Console Scilab"
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

BOOL ScilabIsStarting=TRUE;
BOOL WindowMode;
extern int getdiary();
void C2F(diary) __PARAMS((char *str,int *n));
void diary_nnl __PARAMS((char *str,int *n));

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
void RenameConsole(void)
{
	char CurrentConsoleName[MAX_PATH];
	char CurrentConsoleNameTmp[MAX_PATH];

	GetConsoleTitle(CurrentConsoleName,MAX_PATH);
	strncpy(CurrentConsoleNameTmp,CurrentConsoleName,strlen(NameConsole));
	CurrentConsoleNameTmp[strlen(NameConsole)]='\0';

	if ( strcmp(CurrentConsoleNameTmp,NameConsole) != 0)	 
	{
		wsprintf(ScilexConsoleName,"%s",NameConsole);
		SetConsoleTitle(ScilexConsoleName);
	}
}
/*-----------------------------------------------------------------------------------*/
int Console_Main(int argc, char **argv)
{
  LPSTR tail;
  int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
  char *path = NULL;
  OSVERSIONINFO os;
  int i=0;
  my_argc = -1;

  WindowMode=FALSE;
  
  RenameConsole();

  for (i=0;i<argc;i++)
  {
	  my_argv[i] = argv[i];
  }
  my_argc =argc;



  os.dwOSVersionInfoSize = sizeof (os);
  GetVersionEx (&os);
  SciPlatformId = os.dwPlatformId;

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

  textwin.hInstance = GetModuleHandle (0);
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


	WindowMode=TRUE;
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

	sci_windows_main (nowin, &startupf, path,pathtype, &lpath,memory);

	CloseScilabConsole();
	/* Tue ce process pour fermeture correcte sous Windows 98 */
	Kill_Scilex_Win98();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
void CreateScilabConsole(int ShowBanner)
{
	int Current_Number_of_Scilex=-1; 

	Windows_Console_State=0; /* Console DOS Cachée par défaut */
	AllocConsole();

	Current_Number_of_Scilex=FindFreeScilexNumber();
	wsprintf(ScilexConsoleName,"%s (%d)",NameConsole,Current_Number_of_Scilex);
	SetConsoleTitle(ScilexConsoleName);

	CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
	freopen("CONOUT$", "wb", stderr); /* redirect stderr --> CONOUT$*/
	
	if (ShowBanner)
	{
		char line[80];

		strcpy(line,"        ___________________________________________\r\n");
		printf(line);
	    wsprintf(line,"                         %s\r\n\n",VERSION);
		printf(line);
		strcpy(line,"                  Copyright (c) 1989-2004          \r\n");
		printf(line);
		strcpy(line,"              Consortium Scilab (INRIA, ENPC)      \r\n");
		printf(line);
    	strcpy(line,"        ___________________________________________\r\n\r\n");
		printf(line);
	}

	HideScilex(); /* Cache la fenetre Console */
}
/*-----------------------------------------------------------------------------------*/
void CloseScilabConsole(void)
{
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
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
  hdllInstance=hdllInstanceTmp;
  szModuleName = (LPSTR) malloc (MAXSTR + 1);
  CheckMemory (szModuleName);

  strcpy(szModuleName,p1);
  strcat(szModuleName,"\\bin\\");

  if (CheckWScilabVersion (WSCILABVERSION))
    {
      MessageBox (NULL, "Wrong version of WSCILAB.DLL", szModuleName, MB_ICONSTOP | MB_OK);
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
    
}
/*-----------------------------------------------------------------------------------*/
/* to simulate argv */
void add_sci_argv(char *p)
{
	my_argv[++my_argc]=p;
}
/*-----------------------------------------------------------------------------------*/
/* Fortran iargc and fgetarg implemented here */
int sci_iargc()
{
	#ifdef TEST
	char test[50];
	sprintf(test,"%d",my_argc);
	MessageBox (NULL, test, "pipo", MB_ICONSTOP | MB_OK);
	#endif
	return my_argc -1 ;
}
/*-----------------------------------------------------------------------------------*/
int sci_getarg(int *n,char *s,long int ls)
{
	register char *t;
	register int i;
	#ifdef TEST
	char test[50];
	sprintf(test,"%d %d",my_argc,ls);
	MessageBox (NULL, test, "pipo", MB_ICONSTOP | MB_OK);
	#endif
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
int MyPutCh (int ch)
{
	return TextPutCh (&textwin, (BYTE) ch);
}
/*-----------------------------------------------------------------------------------*/
int MyKBHit (void)
{
	return TextKBHit (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyGetCh (void)
{
	return TextGetCh (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyGetChE (void)
{
	return TextGetChE (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyFGetC (FILE * file)
{
	if (isterm (file))
	{
		return MyGetChE ();
	}
	return fgetc (file);
}
/*-----------------------------------------------------------------------------------*/
char *MyGetS (char *str)
{
	TextPutS (&textwin, "\nDANGER: gets() used\n");
	MyFGetS (str, 80, stdin);
	if (strlen (str) > 0
	&& str[strlen (str) - 1] == '\n')
	str[strlen (str) - 1] = '\0';
	return str;
}
/*-----------------------------------------------------------------------------------*/
char * MyFGetS (char *str, unsigned int size, FILE * file)
{
	char FAR *p;
	if (isterm (file))
	{
		p = TextGetS (&textwin, str, size);
		if (p != (char FAR *) NULL)
		return str;
		return (char *) NULL;
	}
	return fgets (str, size, file);
}
/*-----------------------------------------------------------------------------------*/
int MyFPutC (int ch, FILE * file)
{
	if (isterm (file))
	{
		MyPutCh ((BYTE) ch);
		TextMessage ();
		return ch;
	}
	return fputc (ch, file);
}
/*-----------------------------------------------------------------------------------*/
int MyFPutS (char *str, FILE * file)
{
	if (isterm (file))
	{
		TextPutS (&textwin, str);
		TextMessage ();
		return (*str);		/* different from Borland library */
	}
	return fputs (str, file);
}
/*-----------------------------------------------------------------------------------*/
int MyPutS (char *str)
{
	TextPutS (&textwin, str);
	MyPutCh ('\n');
	TextMessage ();
	return 0;			/* different from Borland library */
}
/*-----------------------------------------------------------------------------------*/
/** synonym for scilab but without the \n **/
void Scistring (char *str)
{
	int i;
	C2F (xscion) (&i);
	if (i == 0)
	fprintf (stdout, "%s", str);
	else
	{
		TextPutS (&textwin, str);
		MyPutCh ('\n');
		TextMessage ();
	}
}
/*-----------------------------------------------------------------------------------*/
int MyFPrintF (FILE * file, char *fmt,...)
{
	int count;
	va_list args;
	va_start (args, fmt);
	if (isterm (file))
	{
		char buf[MAXPRINTF];
		count = vsprintf (buf, fmt, args);
		TextPutS (&textwin, buf);
	}
	else
	count = vfprintf (file, fmt, args);
	va_end (args);
	return count;
}
/*-----------------------------------------------------------------------------------*/
int MyPrintF (char *fmt,...)
{
	int count;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);
	count = vsprintf (buf, fmt, args);
	TextPutS (&textwin, buf);
	va_end (args);
	return count;
}
/*-----------------------------------------------------------------------------------*/
/** Synonym for Scilab of MyPrintf **/
void sciprint (char *fmt,...)
{
	int i, count, lstr;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);

	/* next three lines added for diary SS */
	count = vsprintf (buf, fmt, args);
	lstr = strlen (buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		/*count = vfprintf(stdout, fmt, args ); */
		printf ("%s", buf);

	}
	else
	{
		/*count = vsprintf(buf,fmt,args); SS */
		TextPutS (&textwin, buf);
	}
	if (getdiary()) diary_nnl(buf,&lstr);

	va_end (args);
	/** return count; **/
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* the same but no diary record
*---------------------------------------------------*/
void sciprint_nd (char *fmt,...)
{
	int i, count, lstr;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);

	/* next three lines added for diary SS */
	count = vsprintf (buf, fmt, args);
	lstr = strlen (buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		/*count = vfprintf(stdout, fmt, args ); */
		printf ("%s", buf);

	}
	else
	{
		/*count = vsprintf(buf,fmt,args); SS */
		TextPutS (&textwin, buf);
	}
	va_end (args);
	/** return count; **/
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* as sciprint but with an added first argument
* which is ignored (used in do_printf)
*---------------------------------------------------*/
int sciprint2 (int iv, char *fmt,...)
{
	int i, count,lstr;
	va_list ap;
	char s_buf[1024];
	va_start (ap, fmt);
	/* next three lines added for diary SS */
	count = vsprintf (s_buf, fmt, ap);
	lstr = strlen (s_buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		count = vfprintf (stdout, fmt, ap);
	}
	else
	{
		/* count = vsprintf (s_buf, fmt, ap); SS */
		TextPutS (&textwin, s_buf);
	}
	if (getdiary()) diary_nnl(s_buf,&lstr);

	va_end (ap);
	return count;
}
/*-----------------------------------------------------------------------------------*/
size_t MyFWrite (const void *ptr, size_t size, size_t n, FILE * file)
{
	if (isterm (file))
	{
		int i;
		for (i = 0; i < (int) n; i++)
		TextPutCh (&textwin, ((BYTE *) ptr)[i]);
		TextMessage ();
		return n;
	}
	return fwrite (ptr, size, n, file);
}
/*-----------------------------------------------------------------------------------*/
size_t MyFRead (void *ptr, size_t size, size_t n, FILE * file)
{
	if (isterm (file))
	{
		int i;
		for (i = 0; i < (int) n; i++)
		((BYTE *) ptr)[i] = TextGetChE (&textwin);
		TextMessage ();
		return n;
	}
	return fread (ptr, size, n, file);
}
/*-----------------------------------------------------------------------------------*/
/* I/O Function for scilab : this function are used when Xscilab is on */

/*-----------------------------------------------------------------------------------*/
void Xputchar (c)
char c;
{
	MyPutCh ((int) c);
}
/*-----------------------------------------------------------------------------------*/
void Xputstring (str, n)
char *str;
int n;
{
	int i;
	for (i = 0; i < n; i++)
	Xputchar (str[i]);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscisncr) (str, n, dummy)
char *str;
integer *n, dummy;
{
	int i;
	for (i = 0; i < *n; i++)
	{
		Xputchar (str[i]);
	}
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscistring) (str, n, dummy)
char *str;
int *n;
long int dummy;
{
	int i;
	for (i = 0; i < *n; i++)
	{
		Xputchar (str[i]);
	}
	Xputstring ("\r\n", 2);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscimore) (n)
int *n;
{
	int n1, ln;
	*n = 0;
	ln = strlen (MORESTR);
	Xputstring (MORESTR, ln);
	n1 = TextGetCh (&textwin);
	if (n1 == 'n')
	*n = 1;
	Xputstring ("\r\n", 2);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
/* I/O Function for C routines : test for xscion */

void Scisncr (str)
char *str;
{
	int i;
	integer lstr;
	C2F (xscion) (&i);
	if (i == 0)
	fprintf (stdout, "%s", str);
	else
	{
		lstr = strlen (str);
		C2F (xscisncr) (str, &lstr, 0L);
	}
}
/*-----------------------------------------------------------------------------------*/
/********************************************************************
* int ReadKey (void) - reads console input (including function
*			 			keys and national characters).
* Return value:
*		ASCII character (OEM codepage) in low byte
*		Virtual Keycode of function key in high byte if low byte == 0.
* BUGS:
*		This function does not filter keyboard layout switching
*		hot keys.
*
* Original design by Boris Usievich.
* Custom implementation by Sergey Okhapkin
********************************************************************/
int ReadKey (void)
{
	INPUT_RECORD irBuffer;
	DWORD n;
	int ch;
	HANDLE hIn = GetStdHandle (STD_INPUT_HANDLE);
	SetConsoleMode (hIn, ENABLE_PROCESSED_INPUT);
	do
	{
		PeekConsoleInput (hIn, &irBuffer, 1, &n);
		if (n == 0)
		{
			Sleep (100);
			continue;
		}

		if (irBuffer.EventType == KEY_EVENT &&
		irBuffer.Event.KeyEvent.bKeyDown)
		{
			if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
			{
				ReadConsole (hIn, &ch, 1, &n, NULL);
				return ch & 0xff;
			}
			else
			{
				ReadConsoleInput (hIn, &irBuffer, 1, &n);
				return irBuffer.Event.KeyEvent.wVirtualKeyCode << 8;
			};
		}
		else
		ReadConsoleInput (hIn, &irBuffer, 1, &n);
	}
	while (TRUE);
}
/*-----------------------------------------------------------------------------------*/
int WriteStr (char *s, FILE * file)
{
	#ifdef UseWIN
	BOOL bSuccess;
	DWORD cCharsWritten;
	const PCHAR crlf = "\n";
	BOOL retflag = TRUE;
	HANDLE hOut = GetStdHandle (STD_OUTPUT_HANDLE);
	WriteConsole (hOut, s, strlen (s), &cCharsWritten, NULL);
	#else
	fputs (s, stdout);
	fflush (stdout);
	#endif
	return (strlen (s));
}
/*-----------------------------------------------------------------------------------*/
int WriteKey (char c, FILE * file)
{
	#ifdef UseWIN
	BOOL bSuccess;
	DWORD cCharsWritten;
	const PCHAR crlf = "\n";
	BOOL retflag = TRUE;
	HANDLE hOut = GetStdHandle (STD_OUTPUT_HANDLE);
	SetConsoleMode (hOut, ENABLE_PROCESSED_OUTPUT);
	WriteConsole (hOut, &c, 1, &cCharsWritten, NULL);
	#else
	fputc (c, stdout);
	fflush (stdout);
	#endif
	return (1);
}


/*---------------------------------------------------
* utilities
*---------------------------------------------------*/
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

/********************************************************************************************************/
/* Modification Allan CORNET 09/07/03 */
/* Les variables d'environnements SCI,TCL_LIBRARY,TK_LIBRARY */
/* sont définies directement dans scilex */
/* scilex peut donc etre executé seul */
static void SciEnv ()
{
	char *p;
	char modname[MAX_PATH + 1];
	char env[MAX_PATH + 1 + 10];
	if (!GetModuleFileName (NULL, modname + 1, MAX_PATH))
	return;
	if ((p = strrchr (modname + 1, '\\')) == NULL)
	return;
	*p = 0;

	/* Set SCI variable */
	if ((p = strrchr (modname + 1, '\\')))
	{
		*p = 0;
		for (p = modname + 1; *p; p++)
		{
			if (*p == '\\')	*p = '/';
		}

		p = modname + 1;

		set_sci_env(p,(char *) 0);
/*
		if ((p1 = getenv ("TCL_LIBRARY")) == (char *) 0)
		{
			wsprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.4", p);
			putenv (env);
		}

		if ((p1 = getenv ("TK_LIBRARY")) == (char *) 0)
		{
			wsprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.4", p);
			putenv (env);
		}
*/

		// Bug 763 Pour le moment, on force a utiliser la librairie TCL/TK que l'on embarque
		wsprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.4", p);
		putenv (env);

		wsprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.4", p);
		putenv (env);


	}
}
/********************************************************************************************************/
/*----------------------------------------------------
* set env variables (used when calling scilab from
* other programs)
*----------------------------------------------------*/
void set_sci_env(char *p, char *wsci)
{
	char *p1;
	char env[MAX_PATH + 1 + 10];
	if ((p1 = getenv ("SCI")) == (char *) 0)
	{
		if ( GetVersion() < 0x80000000 )
		{
			/* Windows NT */
			char ShortPath[MAX_PATH];
			GetShortPathName(p ,ShortPath,MAX_PATH);
			sprintf (env, "SCI=%s",ShortPath);
		}
		else
		{
			/* Win32s, Win95,Win98,WinME */
			sprintf (env, "SCI=%s",p);
		}
		putenv (env);
	}
	if ((p1 = getenv ("HOME")) == (char *) 0)
	{
		sprintf (env, "HOME=%s", p);
		putenv (env);
	}
	if ((p1 = getenv ("PWD")) == (char *) 0)
	{
		sprintf (env, "PWD=%s", p);
		putenv (env);
	}
	/** for PVM **/
	for (p1 = p; *p1; p1++)
	{
		if (*p1 == '/')
		*p1 = '\\';
	}
	if ((p1 = getenv ("PVM_ROOT")) == (char *) 0)
	{
		sprintf (env, "PVM_ROOT=%s\\pvm3", p);
		putenv (env);
	}
	if ((p1 = getenv ("PVM_ARCH")) == (char *) 0)
	{
		sprintf (env, "PVM_ARCH=WIN32");
		putenv (env);
	}
	if ((p1 = getenv ("PVM_TMP")) == (char *) 0)
	{
		if ((p1 = getenv ("TEMP")) == (char *) 0)
		{
			char PathTmp[MAX_PATH];
			GetTempPath(MAX_PATH,PathTmp);
			sprintf (env, "PVM_TMP=%s",PathTmp);
			putenv (env);
		}
		else
		{
			sprintf (env, "PVM_TMP=%s", p1);
			putenv (env);
		}
	}
	if ((p1 = getenv ("WSCI")) == (char *) 0)
	{
		if ( wsci != NULL)
		sprintf (env, "WSCI=%s", wsci);
		else
		sprintf (env, "WSCI=%s", p);
		putenv (env);
	}
	
	/* Add lcc to path */
	if ((p1 = getenv ("PATH")) == (char *) 0)
	{
		MessageBox(NULL,"No PATH environment ...","Error",MB_ICONWARNING);
		exit(1);
	}
	else
	{
		char *NewPath;
		char *PathWsci= getenv ("WSCI");
		if ( PathWsci == (char *)0 )
		{
			MessageBox(NULL,"WSCI not defined","Error",MB_ICONWARNING);
			exit(1);
		}
		else
		{
			char PathsLCC[1024];
			char LCCFILE[MAX_PATH];

			wsprintf(LCCFILE,"%s%s",PathWsci,LCCEXE);
			if ( IsAFile(LCCFILE) )
			{
			wsprintf(PathsLCC,"%s%s;%s%s;%s%s",PathWsci,LCCBIN,PathWsci,LCCINCLUDE,PathWsci,LCCLIB);
			NewPath=(char*)malloc( (strlen("PATH=;;")+strlen(p1)+strlen(PathsLCC)+1)*sizeof(char));
			wsprintf(NewPath,"PATH=%s;%s;",p1,PathsLCC);
			putenv (NewPath);
			free(NewPath);
			}
		}
	
	}


	#ifdef __MSC__
	putenv ("COMPILER=VC++");
	#endif
	#if (defined __CYGWIN32__ ) || (defined __MINGW32__)
	putenv ("COMPILER=gcc");
	#endif
	#ifdef __ABSC__
	putenv ("COMPILER=ABSOFT");
	#endif
	putenv ("WIN32=OK");
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* platform flag
*	The return value is one of:
*	    VER_PLATFORM_WIN32s		Win32s on Windows 3.1.
*	    VER_PLATFORM_WIN32_WINDOWS	Win32 on Windows 95.
*	    VER_PLATFORM_WIN32_NT	Win32 on Windows NT
*---------------------------------------------------*/
int SciWinGetPlatformId ()
{
	return SciPlatformId;
}
/*-----------------------------------------------------------------------------------*/
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
/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void)
{
	HWND hScilexN=NULL;
	int Number_of_Scilex=0;
	char NameScilex[MAX_PATH];

	wsprintf(NameScilex,"%s (%d)",NameConsole,Number_of_Scilex);
	while ( FindWindow(NULL,NameScilex) )
	{
		Number_of_Scilex++;
		wsprintf(NameScilex,"%s (%d)",NameConsole,Number_of_Scilex);
	}

	return Number_of_Scilex;
}
/*-----------------------------------------------------------------------------------*/
/* teste si la chaine de caractere correspond à un fichier*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsAFile(char *chainefichier)
{
     BOOL retour=FALSE;	
	
     WIN32_FIND_DATA FindFileData;
     HANDLE handle = FindFirstFile (chainefichier, &FindFileData);
     if (handle != INVALID_HANDLE_VALUE)
     {
         FindClose (handle);
         retour=TRUE;
     }
     else retour=FALSE;

     return retour;
}
/*-----------------------------------------------------------------------------------*/
/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec les extension BIN et SAV */
	if ( (strcmp(lastdot,"BIN")==0) || (strcmp(lastdot,"SAV")==0) ) retour=TRUE;

	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAGraphFilegraphb(chainefichier) || IsAGraphFilegraph(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraph(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	unsigned int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}

	/* Comparaison avec l'extension Graph */
	if ( strcmp(lastdot,"GRAPH")==0) retour=TRUE;

	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraphb(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	
	/* Comparaison avec l'extension Graph */
	if ( strcmp(lastdot,"GRAPHB")==0) retour=TRUE;

	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFile(char *chainefichier)
{
	BOOL retour=FALSE;
	
	if ( IsAScicosFileCOS(chainefichier) || IsAScicosFileCOSF(chainefichier) ) retour=TRUE;
	
	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFileCOS(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}
	/* Mise en majuscule de l'extension du fichier*/
	if (lastdot == NULL) return retour;

	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension COS */
	
	if ( strcmp(lastdot,"COS")==0 ) retour=TRUE;

	return retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsAScicosFileCOSF(char *chainefichier)
{
	char ChaineTemp[MAX_PATH];
	char *buffer=NULL;
	char *lastdot=NULL;
	int i=0;
	BOOL retour=FALSE;

	strcpy(ChaineTemp,chainefichier);
	/* Recherche de l'extension du fichier */
	buffer=strtok(ChaineTemp,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}

	if (lastdot == NULL) return retour;

	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<(int)strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension COSF */
	if ( strcmp(lastdot,"COSF")==0 ) retour=TRUE;

	return retour;
}
/*-----------------------------------------------------------------------------------*/
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd)
{
	int Retour=FALSE;
	char FinalFileName[MAX_PATH];
	char ShortPath[MAX_PATH];
	char PathWScilex[MAX_PATH];

	
	if (fichier[0]=='\"')
	{
		char buffertemp[MAX_PATH];
		int i=1;
		
		while (fichier[i] != '"')
		{
			buffertemp[i-1]=fichier[i];
			i++;
			if (i> (int)strlen(fichier))
			{
				i=(int)strlen(fichier);
				break;
			}
		}
		buffertemp[i-1]='\0';
		strcpy(fichier,buffertemp);
	}
	if (fichier[strlen(fichier)-1]=='\"') fichier[strlen(fichier)-1]='\0';

	if (IsAFile(fichier))
	{
		GetShortPathName(fichier,ShortPath,MAX_PATH); /* Recuperation du nom du fichier au format 8.3 */
		ReplaceSlash(FinalFileName,ShortPath);
		GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), PathWScilex, MAX_PATH);
		Retour=TRUE;

		switch (OpenCode)
		   {
			case 1: /* Execute -X*/
				{
					if ( IsABinOrSavFile(FinalFileName)== TRUE )
					{
						/* C'est un fichier .BIN ou .SAV d'ou load */
						wsprintf(Cmd,"%s -e load('%s');disp('\"\"%s\"\"loaded'); ",PathWScilex,FinalFileName,FinalFileName);
					}
					else
					if  ( IsAScicosFile(fichier)== TRUE )
					{
						ReplaceSlash(FinalFileName,fichier);
						ExtensionFileIntoLowerCase(FinalFileName);	
						/*wsprintf(Cmd,"%s -e scicos('%s');disp('\"\"%s\"\"loaded'); ",PathWScilex,FinalFileName,FinalFileName);*/
						wsprintf(Cmd,"%s -e scicos('%s'); ",PathWScilex,FinalFileName,FinalFileName);
					}
					else
					if ( IsAGraphFile(fichier)== TRUE )
					{
						ReplaceSlash(FinalFileName,fichier);
						ExtensionFileIntoLowerCase(FinalFileName);	
						/*wsprintf(Cmd,"%s -e edit_graph('%s');disp('\"\"%s\"\"loaded'); ",PathWScilex,FinalFileName,fichier);*/
						wsprintf(Cmd,"%s -e edit_graph('%s'); ",PathWScilex,FinalFileName,fichier);
					}
					else wsprintf(Cmd,"%s -e exec('%s'); ",PathWScilex,FinalFileName);
				}
			break;
			case 2: /* Print -P*/
				{
					/* A mettre en place plus precisément*/
					/* Pour le moment l'impression a lieu par l'editeur de fichier .txt défini par défaut */
					/* c-a-d notepad ou autre */
				PrintFile(fichier);
				strcpy(Cmd," ");
				exit(0);


				}
			break;
			case 0:default: /* Open -O*/
				{
					if (! HaveAnotherWindowScilab())
					{
						wsprintf(Cmd,"%s -e scipad('%s'); ",PathWScilex,FinalFileName);
					}
					else
					{
						char *ScilabDestination=NULL;
						wsprintf(Cmd,"scipad('%s'); ",FinalFileName);

						ScilabDestination=(char*)ChooseAnotherWindowScilab();
						if (ScilabDestination)
						{
							SendCommandToAnotherScilab("Scilab Communication",ScilabDestination,Cmd);
							free(ScilabDestination);
							exit(0);
						}
						else
						{
							wsprintf(Cmd,"%s -e scipad('%s'); ",PathWScilex,FinalFileName);
						}
						
					}
				}
			break;
		
	}
	
	}	
	
	
	return Retour;
}
/*-----------------------------------------------------------------------------------*/
void ExtensionFileIntoLowerCase(char *fichier)
{
	char *tmpfile=NULL;
	char *buffer=NULL;
	char *lastdot=NULL;
	char *ext=NULL;
	
	tmpfile=(char*)malloc(strlen(fichier)*sizeof(char));
	strcpy(tmpfile,fichier);
	
	buffer=strtok(tmpfile,".");
	while ( buffer = strtok(NULL,"."))
	{
		lastdot=buffer;
	}
	/* le dernier . permet d'avoir l'extension */
	ext=_strlwr(lastdot); /* Fichier en Majuscule */
	
	strcpy(&fichier[strlen(fichier)-strlen(ext)],ext);
	
	free(tmpfile);
}
/*-----------------------------------------------------------------------------------*/
BOOL IsWindowInterface()
{
	return WindowMode;
}
/*-----------------------------------------------------------------------------------*/
