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

BOOL WindowMode;
extern int getdiary();
void C2F(diary) __PARAMS((char *str,int *n));
void diary_nnl __PARAMS((char *str,int *n));


/*-----------------------------------------------------------------------------------*/
void InitWindowGraphDll(void)
/* Modification Allan CORNET*/
/* Novembre 2003 */
/* Permet l'affichage des menus graphiques lors d'un appel de la DLL Scilab*/
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
/*---------------------------------------------------
* The WinMain function
*---------------------------------------------------*/

int MyWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
LPSTR szCmdLine, int iCmdShow)
{
	WinMain (hInstance,hPrevInstance, szCmdLine,iCmdShow);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
#ifndef __ABSC__
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
#else
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
#endif
{
	char *pFullCmdLine=NULL;
	char CmdlineToTransmit[MAX_PATH];
	char CmdlineToTransmitTmp[MAX_PATH];
	int Current_Number_of_Scilex=-1;
	LPSTR tail;
	int nowin = 0, argcount = 0, lpath = 0, pathtype=0;
	int i=0;
	int nbarg=0;
	char FileToEdit[MAX_PATH];
	char *path = NULL;
	OSVERSIONINFO os;
        int CodeAction=-1;

	WindowMode=TRUE;
	my_argc = -1;
	
	pFullCmdLine=GetCommandLine();
	//strcpy(CmdlineToTransmit,pFullCmdLine);
	strcpy(CmdlineToTransmit,szCmdLine);
	strcpy(CmdlineToTransmitTmp,szCmdLine);
	
	my_argv[++my_argc] = strtok (szCmdLine, " ");
	while (my_argv[my_argc] != NULL)
	{
		my_argv[++my_argc] = strtok(NULL, " ");
		
	}
	
	/* Sauvegarde la ligne de commande */
	if ( ! SaveArgs(CmdlineToTransmit) ) strcpy(CmdlineToTransmit,CmdlineToTransmitTmp);
	
	
	/* Teste et prepare la ligne de commande*/
	/* Donne leChoix utilisateur dans CodeAction "Open , Execute , Print" */
	if ( CleanFileName(CmdlineToTransmit,FileToEdit,&CodeAction) == TRUE )
	{
		strcpy(CmdlineToTransmit,FileToEdit);
	
		nbarg=1;
	}
	

	os.dwOSVersionInfoSize = sizeof (os);
	GetVersionEx (&os);
	SciPlatformId = os.dwPlatformId;

	szModuleName = (LPSTR) malloc (MAXSTR + 1);
	CheckMemory (szModuleName);
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
	strcat (textwin.AboutText, "Scilab is a free copyrighted software.\n\t");
	strcpy (textwin.AboutText, "developed by Scilab Group (Inria/Enpc)\n\t");
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
	argcount = my_argc;
	/* Modification Allan CORNET le 22 Juillet 2003 */
	/* Permet l'association pour ouverture directement avec scipad */

	if ( (nbarg== 1)&& (IsAFile(FileToEdit) == TRUE) )
	{
		char Commande[MAX_PATH];
		
		CommandByFileExtension(FileToEdit,CodeAction,Commande);
		if (CodeAction==2) exit(1); /*Sortie apres impression */
		
		path = Commande;
		lpath = strlen (Commande);
		pathtype=1;
	}
	else
	{
	
	while (argcount > 0)
	{
                char ArgTmp[MAX_PATH];
                
		argcount--;
		strcpy(ArgTmp,my_argv[argcount]);
		if ( (strcmp (strupr(ArgTmp), "-H") == 0) || (strcmp (ArgTmp, "-?") == 0) || (strcmp (strupr(ArgTmp), "-HELP") == 0) )
		{

			MessageBox(NULL,"-h help\n-f <script> Execute script file\n-nwni Console Mode\n-nb no banner\n-e\n-mem\n-nw\n-ns","Help",MB_ICONINFORMATION);

			exit(1);
		}
		
		if (strcmp (strupr(ArgTmp), "-NW") == 0)
		{
			nowin = 1;
		}
		else if (strcmp (strupr(ArgTmp), "-NS") == 0)
		startupf = 1;
		else if ( strcmp(strupr(ArgTmp),"-NB") == 0) { sci_show_banner = 0; }
		else if (strcmp (strupr(ArgTmp), "-NWNI") == 0)
		{
			nowin = 1;
			nointeractive = 1;
		}
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
	}
	#ifndef __DLL__
	/** when we don't use a dll version of the graphic library
	which is the case up to now **/
	NoDll (hInstance);
	#endif
	atexit (WinExit);
	SciEnv ();
	if (nowin == 1)
	{
		WindowMode=FALSE;
		/** XXXX AllocConsole(); **/
		sci_windows_main (nowin, &startupf,path,pathtype, &lpath,memory);
	}
	else
	{

		/* Mode Interface Windows */
		STARTUPINFO InfoCurrentProcess; /* Utiliser pour récuperer le nom de la Console Courante sous W9x*/

		Current_Number_of_Scilex=FindFreeScilexNumber();

		/* Cree un process independant si Scilex est lancé depuis le shell */
		/* Permet d'eviter d'avoir à utiliser runscilab cf. Version 2.7 */
		
		CreateProcessScilex(CmdlineToTransmit,Current_Number_of_Scilex,CodeAction);

		/* Retrouve le nom de la fenetre Console pour Windows 9x */
		if (SciWinGetPlatformId () == VER_PLATFORM_WIN32_WINDOWS)
		{
			GetStartupInfo(&InfoCurrentProcess); /* Recupere le nom de la Console Courante */
			strcpy(ScilexConsoleName,InfoCurrentProcess.lpTitle); /* Sauvegarde du nom de la console */
		}

		if (TextInit (&textwin))	exit (1);

		/* Modification Allan CORNET 18/07/03 */
		/* Splashscreen*/
		if (sci_show_banner) CreateThreadSplashscreen();

		textwin.hIcon = LoadIcon (hInstance, "texticon");
		SetClassLong (textwin.hWndParent, GCL_HICON, (DWORD) textwin.hIcon);

		SetXsciOn ();
		
		ShowWindow (textwin.hWndParent, SW_SHOW);
		
		sci_windows_main (nowin, &startupf, path,pathtype, &lpath,memory);

		/* Tue ce process pour fermeture correcte sous Windows 98 */
		Kill_Scilex_Win98();

	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
#define __MSC_NOC__
#ifdef __MSC_NOC__
/*
we want Scilab to work in two mode
as a console application or as a window
application
for a console application
we do not directly use main for scilab
since main is in libf2c which in truns call our MAIN__
* */

int MAIN__ ()
{
	#ifndef __ABSC__
	HANDLE x = GetModuleHandleA (0);
	WinMain (x, 0, GetCommandLineA (), 1);
	#else
	HMODULE x = GetModuleHandle (0);
	WinMain (x, 0, GetCommandLine (), 1);
	#endif
	return (0);
}
#endif
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
	if (str == (LPSTR) NULL)
	{
		MessageBox (NULL, "out of memory", "Scilab", MB_ICONSTOP | MB_OK);
		exit (1);
	}
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
	char *p, *p1;
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

		if ((p1 = getenv ("TCL_LIBRARY")) == (char *) 0)
		{
			wsprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.3", p);
			putenv (env);
		}

		if ((p1 = getenv ("TK_LIBRARY")) == (char *) 0)
		{
			wsprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.3", p);
			putenv (env);
		}

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
		sprintf (env, "SCI=%s",p);
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
			sprintf (env, "PVM_TMP=c:\\tmp");
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
/* Allan CORNET 17/07/03 */
/* Cree un process independant si Scilex est lancé depuis le shell */
/* Permet d'eviter d'avoir à utiliser runscilab cf. Version 2.7 */
void CreateProcessScilex(char *cmdline,int NumberScilex,int CodeActionWin98)
{
	char modname[MAX_PATH + 1];
	
	if ( ! GetModuleFileName (NULL, modname, MAX_PATH) ) exit (1);
	else
	{
		if (SciWinGetPlatformId () == VER_PLATFORM_WIN32_WINDOWS)
		{
			HWND hConsole;

			char TitleConsole[MAX_PATH];

			hConsole=FindWindow(NULL,"scilex");

			if (hConsole)
			{

				STARTUPINFO start;
				SECURITY_ATTRIBUTES sec_attrs;
				PROCESS_INFORMATION child;

				SetWindowText(hConsole,"");
				memset (&start, 0, sizeof (start));
				start.cb = sizeof (start);

				wsprintf(TitleConsole,"%s (%d)","Scilex", NumberScilex);
				start.lpTitle=TitleConsole;
				
				if ( IsAFile(cmdline) )
				{
					char bufftemp[MAX_PATH];
					wsprintf(bufftemp,"\"%s\"",cmdline);
					strcpy(cmdline,bufftemp);
				}
					

				strcat(modname," ");
				
				strcat(modname,cmdline);
				
				switch (CodeActionWin98)
				{
					case 0:
						strcat(modname," -o");
					break;
					case 1:
						strcat(modname," -x");
					break;
					case 2:
						strcat(modname," -p");
					break;
					default:
					break;
				}
				strcat(modname," &\0");

				start.dwFlags = STARTF_USESHOWWINDOW;

				start.wShowWindow = SW_NORMAL;


				sec_attrs.nLength = sizeof (sec_attrs);
				sec_attrs.lpSecurityDescriptor = NULL;
				sec_attrs.bInheritHandle = FALSE;

				if (CreateProcess (NULL,modname, &sec_attrs, NULL, FALSE, CREATE_NEW_CONSOLE,  NULL, NULL, &start, &child))
				{
					CloseHandle (child.hThread);
					CloseHandle (child.hProcess);
				}
				exit (1);
			}

		}
		else
		{
			char ConsoleName[MAX_PATH];
			GetConsoleTitle(ConsoleName,MAX_PATH);

			if (strcmp(ConsoleName,modname)==0)
			{
				/* Execution directe*/
				HWND hConsole;
				char TitleConsole[MAX_PATH];

				hConsole=FindWindow(NULL,ConsoleName);
				wsprintf(TitleConsole,"%s (%d)","Scilex", NumberScilex);
				strcpy(ScilexConsoleName,TitleConsole);
				SetWindowText(hConsole,TitleConsole);
			}
			else
			{

				/* Ligne de commandes ou raccourci */
				/* Creation d'un process separé */

				STARTUPINFO start;
				SECURITY_ATTRIBUTES sec_attrs;
				PROCESS_INFORMATION child;

				memset (&start, 0, sizeof (start));
				start.cb = sizeof (start);
				start.dwFlags = STARTF_USESHOWWINDOW;

				start.wShowWindow = SW_SHOWMINIMIZED;

				sec_attrs.nLength = sizeof (sec_attrs);
				sec_attrs.lpSecurityDescriptor = NULL;
				sec_attrs.bInheritHandle = FALSE;
	
				/* Creation d'un nouveau process scilex independant du shell */
				if (CreateProcess (modname,cmdline, &sec_attrs, NULL, FALSE, CREATE_NEW_CONSOLE,  NULL, NULL, &start, &child))
				{
					CloseHandle (child.hThread);
					CloseHandle (child.hProcess);
				}

				/* Fermeture du process scilex courant*/
				/* pour laisser place a celui qui est autonome de la ligne de commande */
				exit (1);

			}
		}
	}

}
/*-----------------------------------------------------------------------------------*/
void CreateThreadSplashscreen(void)
{
	HANDLE hThread;
	DWORD IdThread;
	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadSplashscreen,(LPVOID)NULL,0,(LPWORD)&IdThread);
	CloseHandle( hThread );
}
/*-----------------------------------------------------------------------------------*/
DWORD WINAPI ThreadSplashscreen(LPVOID lpParam )
{
	HWND hdlg;
	char *buffer=NULL;

	buffer=(char*)malloc(strlen("Copyright ® ")+strlen(DEFAULT_MES)+1);
	wsprintf(buffer,"%s %s","Copyright ® ",DEFAULT_MES);

	hdlg = CreateDialog(hdllInstance, "IDD_SPLASH", NULL,NULL);
	SetDlgItemText(hdlg,IDC_VERSION_SPLASH,VERSION);
	SetDlgItemText(hdlg,IDC_COPYRIGHT_SPLASH,buffer);
	free(buffer);
	ShowWindow(hdlg, SW_SHOWDEFAULT);
	UpdateWindow(hdlg);
	Sleep(1500);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Retourne un numéro valide pour nommer les fenetres associées à ce process */
int FindFreeScilexNumber(void)
{
	HWND hScilexN=NULL;
	int Number_of_Scilex=0;
	char NameScilex[MAX_PATH];

	wsprintf(NameScilex,"%s (%d)","Scilex",Number_of_Scilex);
	while ( FindWindow(NULL,NameScilex) )
	{
		Number_of_Scilex++;
		wsprintf(NameScilex,"%s (%d)","Scilex",Number_of_Scilex);
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
	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec les extension BIN et SAV */
	if ( (strcmp(lastdot,"BIN")==0) || (strcmp(lastdot,"SAV")==0) ) retour=TRUE;

	return retour;

}
/*-----------------------------------------------------------------------------------*/
BOOL IsAGraphFilegraph(char *chainefichier)
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
	for (i=0;i<strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension Graph */
	if ( strcmp(lastdot,"GRAPH")==0) retour=TRUE;

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
	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<strlen(lastdot);i++)
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
	for (i=0;i<strlen(lastdot);i++)
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
	/* Mise en majuscule de l'extension du fichier*/
	for (i=0;i<strlen(lastdot);i++)
	{
		lastdot[i]=toupper(lastdot[i]);
	}
	/* Comparaison avec l'extension COSF */
	if ( strcmp(lastdot,"COSF")==0 ) retour=TRUE;

	return retour;
}
/*-----------------------------------------------------------------------------------*/
/* Sauvegarde des parametres eventuels de la ligne de commandes */
int SaveArgs(LPSTR saveargv)
{
	int CodeRetour=TRUE;
	char tmpsaveargv[MAX_PATH];
	char params[MAX_PATH];
	char *ScilexName=NULL;
	char *Strfind=NULL;
	/*char ScilexName[11];*/
	int i=0;
	int pos=0;
	

	strcpy(tmpsaveargv,saveargv);
	
	ScilexName=strupr(tmpsaveargv);
	/* recherche scilex.exe  dans la ligne passée*/
	/* Strfind chaine en majuscule */
	Strfind=strstr(ScilexName,ExecutableNameOfScilab);
	
	if (Strfind) /* Nom de l'executable trouvé */
	{
		pos=10; /*nombre de caracteres composants "scilex.exe" avec \0 à la fin */
		strcpy(tmpsaveargv,Strfind);
		
		if (strlen(tmpsaveargv)>10) /* il y a des parametres */
		{
			if (tmpsaveargv[pos] == '\"') pos++;
			if (tmpsaveargv[pos] == ' ') pos++;
			for (i=pos;i<strlen(tmpsaveargv);i++)
			{
				params[i-pos]=tmpsaveargv[i];
			}
			params[i-pos]='\0';
		
		}
		else strcpy(params,""); /* Pas de Parametres */
	
	}
	else
	{
		strcpy(params,"");
		CodeRetour=FALSE;
	}
	strcpy(saveargv,params);

	return CodeRetour;
}
/*-----------------------------------------------------------------------------------*/
/* Teste si la chaine de caracteres line correspond à un fichier existant */
/* retourne le nom du fichier au format 8.3 pour Scipad */
/* retourne TRUE si OK */
int CleanFileName(char *line,char *filename,int *option)
{
	int Retour=FALSE;
	char buffertemp[MAX_PATH];
	int i=0;
	
	*option=-1;
	
	strcpy(buffertemp,line); /* Copie de line pour modification */
	
	/* Windows passe le nom des fichiers entre "" lorsque l'extension a été associé 
	dans la base de registres */
	if (buffertemp[0] == '"') /* On enleve les " au debut et à la fin */
	{
		
		char stroption[3];
		
		i=1;
		
		while (line[i] != '"')
		{
			buffertemp[i-1]=line[i];
			i++;
		}
		buffertemp[i-1]='\0';
		
		stroption[0]=line[i+2];
		stroption[1]=line[i+3];
		stroption[2]='\0';
		
		
		if  ( (strcmp(stroption,"-o") == 0) || (strcmp(stroption,"-O") == 0) )
		{
			*option=0; /* Code pour Open */
		}
		else
		if ( (strcmp(stroption,"-x") == 0) || (strcmp(stroption,"-X") == 0) )
		{
			*option=1; /* Code pour Execute */
		}
		else
		if ( (strcmp(stroption,"-p") == 0) || (strcmp(stroption,"-P") == 0) )
		{
			*option=2; /* Code pour Print */
		}
		else *option=-1; /* Code pour le reste */

		strcpy(line,buffertemp); 
		
		
		if  ( (IsAGraphFile(line) == TRUE) || (IsAScicosFile(line) == TRUE) )
		{
			strcpy(filename,line); 
			Retour=IsAFile(line);
		}
		else
		{
			GetShortPathName(line,filename,MAX_PATH); /* Recuperation du nom du fichier au format 8.3 */
			Retour=IsAFile(filename);
		}
		
		
		
	}
	else /* pas de " */
	{
		/*GetShortPathName(line,filename,MAX_PATH); /* Recuperation du nom du fichier au format 8.3 */
		/*Retour=IsAFile(filename);*/
		
		Retour=FALSE;
	}

	return Retour;
	
	
	
}
/*-----------------------------------------------------------------------------------*/
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd)
{
	int Retour=FALSE;
	char chemin[MAX_PATH];
	

	/* Il faut doubler les \ pour transmettre chemin correctement à Scipad*/
	DoubleDoubleSlash(chemin,fichier);
		
	if ( IsABinOrSavFile(fichier)== TRUE )
	{

		/* C'est un fichier .BIN ou .SAV d'ou load */
		wsprintf(Cmd,"load('%s');disp('%s loaded');",fichier,fichier);
	}
	else
	   {
		/* Autre fichier .SCE ou .SCI d'ou Ouverture suivant code */
		switch (OpenCode)
		   {
			
			case 1: /* Execute */
			{
				if  ( IsAScicosFile(fichier)== TRUE )
				{
					ExtensionFileIntoLowerCase(fichier);	
					wsprintf(Cmd,"scicos('%s');disp('%s loaded');",fichier,fichier);
				}
				else
				if ( IsAGraphFile(fichier)== TRUE )
				{
			
					ExtensionFileIntoLowerCase(fichier);	
					wsprintf(Cmd,"edit_graph('%s');disp('%s loaded');",fichier,fichier);
			
				}
				else
				wsprintf(Cmd,"exec('%s');",fichier);
			}
			break;
			
			case 2: /* Print */
			/* A mettre en place plus precisément*/
			/* Pour le moment l'impression a lieu par l'editeur de fichier .txt défini par défaut */
			/* c-a-d notepad ou autre */
			{
				MessageBox(NULL,"En cours de Dév.","Impression fichier",MB_OK);
				
					
				/*
				PrintFileText(fichier);*/
				strcpy(Cmd,"");
			}
			break;
			case 0: case -1: /* Edit pour toute autre association*/
			wsprintf(Cmd,"scipad('%s')",chemin);
			break;
			default: /* Probleme */
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
void PrintFileText(char *fichier)
{
	
	PRINTDLG pd;
	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = NULL;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC; 
	pd.nCopies     = 1;
	pd.nFromPage   = 0xFFFF; 
	pd.nToPage     = 0xFFFF; 
	pd.nMinPage    = 1; 
	pd.nMaxPage    = 0xFFFF; 

	if (PrintDlg(&pd)==TRUE) 
	{
    		// GDI calls to render output. 
    		int i=0;
    		int error;
		for (i=0;i<pd.nCopies;i++)
		{
			error =(int)ShellExecute(NULL, "print",fichier,NULL,NULL,SW_SHOWMINIMIZED);
	    		if (error<= 32) MessageBox(NULL,"Couldn't print via notepad","Warning",MB_ICONWARNING);
		}

    		// Delete DC when done.
    		DeleteDC(pd.hDC);
	}
	
}
/*-----------------------------------------------------------------------------------*/
