
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
 * Modified for Scilab (1997) : Jean-Philippe Chancelier 
 * 
 */

#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>

#ifndef __GNUC__
#include <commctrl.h>
#endif

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef __MSC__
#define putenv(x) _putenv(x)
#endif

#ifdef __ABSC__
#define putenv(x) abs_putenv(x)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "plot.h"
#include "wgnuplib.h"
#include "wtext.h"
#include "wcommon.h"

extern void C2F (tmpdirc) ();

/* limits */
#define MAXSTR 255
#define MAXPRINTF 1024

/* globals */
TW textwin;
GW graphwin;
PW pausewin;
MW menuwin;

LPSTR szModuleName;
LPSTR szMenuName;
LPSTR szGraphMenuName;

int SciPlatformId;


#define MENUNAME "wscilab.mnu"
#define GRAPHMENUNAME "wgscilab.mnu"

void WinExit (void);

extern void C2F (getwins) (integer *, integer *, integer *);
extern void C2F (diary) (char *, int *, int);

static void AllGraphWinDelete ();
static int startupf = 0;
/** 0 if we execute startup else 1 **/
static int nointeractive = 0;

static void 
CheckMemory (LPSTR str)
{
  if (str == (LPSTR) NULL)
    {
      MessageBox (NULL, "out of memory", "Scilab", MB_ICONSTOP | MB_OK);
      exit (1);
    }
}

int 
Pause (LPSTR str)
{
  pausewin.Message = str;
  return (PauseBox (&pausewin) == IDOK);
}

/**************************
 * atexit procedure for scilab and scilab -nw 
 *************************/

void 
WinExit (void)
{
  int i;
/** TMPDIR is not created with scilab -ns **/
  if (startupf == 0)
    C2F (tmpdirc) ();
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

/**************************
 * procedure called in sciquit 
 * but WinExit do the proper job at exit
 *************************/

int C2F (clearexit) (integer * n)
{
  return (0);
}


static void 
AllGraphWinDelete ()
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

/* call back function from Text Window WM_CLOSE */

EXPORT int CALLBACK 
ShutDown (void)
{
  exit (0);
  return 0;
}

/********************************
 * Set up environment variables if 
 * necessary 
 ********************************/

static void 
SciEnv ()
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
	  if (*p == '\\')
	    *p = '/';
	}
#ifdef __CYGWIN32__
      if (modname[2] == ':')
	{
	  modname[2] = modname[1];
	  modname[0] = '/';
	  modname[1] = '/';
	  p = modname;
	}
      else
	{
	  p = modname + 1;
	}
#else
      p = modname + 1;
#endif
      if ((p1 = getenv ("SCI")) == (char *) 0)
	{
	  sprintf (env, "SCI=%s", p);
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
      if ((p1 = getenv ("MANCHAPTERS")) == (char *) 0)
	{
	  sprintf (env, "MANCHAPTERS=%s/man/Chapters", p);
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
	  sprintf (env, "WSCI=%s", p);
	  putenv (env);
	}
#ifdef __MSC__
      putenv ("COMPILER=VC++");
#endif
#ifdef __CYGWIN32__
      putenv ("COMPILER=gcc");
#endif
#ifdef __ABSC__
      putenv ("COMPILER=ABSOFT");
#endif
      putenv ("WIN32=OK");
    }
}

/*************
 * platform flag 
 *	The return value is one of:
 *	    VER_PLATFORM_WIN32s		Win32s on Windows 3.1. 
 *	    VER_PLATFORM_WIN32_WINDOWS	Win32 on Windows 95.
 *	    VER_PLATFORM_WIN32_NT	Win32 on Windows NT
 ****************/

int 
SciWinGetPlatformId ()
{
  return SciPlatformId;
}


/***************************************
 * The WinMain function 
 ***************************************/

#ifndef __ABSC__
int WINAPI 
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 PSTR szCmdLine, int iCmdShow)
#else
int WINAPI 
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	 LPSTR szCmdLine, int iCmdShow)
#endif
{
  LPSTR tail;
  int nowin = 0, argcount = 0, lpath = 0;
  char *path = NULL;
  OSVERSIONINFO os;
#if (defined __GNUC__) || (defined __ABSC__)	/* arguments are in szCmdline */
#define MAXCMDTOKENS 128
  int _argc = -1;
  LPSTR _argv[MAXCMDTOKENS];
  _argv[++_argc] = strtok (szCmdLine, " ");
//  fprintf(stderr,"argv[%d] = %s\n",_argc,_argv[_argc]);
  while (_argv[_argc] != NULL)
    {
      _argv[++_argc] = strtok (NULL, " ");
      //   fprintf(stderr,"argv[%d] = %s\n",_argc,_argv[_argc]);
    }
#else
/** VC++  doesn't give us _argc and _argv[] so ...   */
#define MAXCMDTOKENS 128
  int _argc = 0;
  LPSTR _argv[MAXCMDTOKENS];
  _argv[_argc] = "scilex.exe";
  _argv[++_argc] = strtok (szCmdLine, " ");
  while (_argv[_argc] != NULL)
    _argv[++_argc] = strtok (NULL, " ");
#endif
/** __GNUC__ **/

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
  textwin.KeyBufSize = 2048;
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
  argcount = _argc;
  while (argcount > 0)
    {
      argcount--;
      if (strcmp (_argv[argcount], "-nw") == 0)
	nowin = 1;
      else if (strcmp (_argv[argcount], "-ns") == 0)
	startupf = 1;
      else if (strcmp (_argv[argcount], "-nwni") == 0)
	{
	  nowin = 1;
	  nointeractive = 1;
	}
      else if (strcmp (_argv[argcount], "-f") == 0 && argcount + 1 < _argc)
	{
	  path = _argv[argcount + 1];
	  lpath = strlen (_argv[argcount + 1]);
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
/** XXXX AllocConsole(); **/
      sci_windows_main (nowin, &startupf, path, &lpath);
    }
  else
    {
      if (TextInit (&textwin))
	exit (1);
      textwin.hIcon = LoadIcon (hInstance, "texticon");
      SetClassLong (textwin.hWndParent, GCL_HICON, (DWORD) textwin.hIcon);
      SetXsciOn ();
/** XXX **/
      ShowWindow (textwin.hWndParent, SW_SHOWNORMAL);
      sci_windows_main (nowin, &startupf, path, &lpath);
    }
  return 0;
}
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

int 
MAIN__ ()
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


/* replacement stdio routines that use Text Window for stdin/stdout */
/* WARNING: Do not write to stdout/stderr with functions not listed 
   in win/wtext.h */

#undef kbhit
#undef getche
#undef getch
#undef putch

#undef fgetc
#undef getchar
#undef getc
#undef fgets
#undef gets

#undef fputc
#undef putchar
#undef putc
#undef fputs
#undef puts

#undef fprintf
#undef printf
#undef vprintf
#undef vfprintf

#undef fwrite
#undef fread

/** test if we are using a file or not **/

#define isterm(f) (f==stdin || f==stdout || f==stderr)

int 
InteractiveMode ()
{
  return nointeractive;
}

int
MyPutCh (int ch)
{
  return TextPutCh (&textwin, (BYTE) ch);
}

int
MyKBHit (void)
{
  return TextKBHit (&textwin);
}

int
MyGetCh (void)
{
  return TextGetCh (&textwin);
}


int
MyGetChE (void)
{
  return TextGetChE (&textwin);
}

int
MyFGetC (FILE * file)
{
  if (isterm (file))
    {
      return MyGetChE ();
    }
  return fgetc (file);
}

char *
MyGetS (char *str)
{
  TextPutS (&textwin, "\nDANGER: gets() used\n");
  MyFGetS (str, 80, stdin);
  if (strlen (str) > 0
      && str[strlen (str) - 1] == '\n')
    str[strlen (str) - 1] = '\0';
  return str;
}

char *
MyFGetS (char *str, unsigned int size, FILE * file)
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

int
MyFPutC (int ch, FILE * file)
{
  if (isterm (file))
    {
      MyPutCh ((BYTE) ch);
      TextMessage ();
      return ch;
    }
  return fputc (ch, file);
}

int
MyFPutS (char *str, FILE * file)
{
  if (isterm (file))
    {
      TextPutS (&textwin, str);
      TextMessage ();
      return (*str);		/* different from Borland library */
    }
  return fputs (str, file);
}

int
MyPutS (char *str)
{
  TextPutS (&textwin, str);
  MyPutCh ('\n');
  TextMessage ();
  return 0;			/* different from Borland library */
}

/** synonym for scilab but without the \n **/

void 
Scistring (char *str)
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

int 
MyFPrintF (FILE * file, char *fmt,...)
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

int 
MyPrintF (char *fmt,...)
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

/** Synonym for Scilab of MyPrintf **/

void 
sciprint (char *fmt,...)
{
  int i, count, lstr;
  char buf[MAXPRINTF];
  va_list args;
  va_start (args, fmt);

  /* next three lines added for diary SS */
  count = vsprintf (buf, fmt, args);
  lstr = strlen (buf);
  C2F (diary) (buf, &lstr, 0L);

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

/* 
   as sciprint but with an added first argument 
   which is ignored (used in do_printf) 
 */

int 
sciprint2 (int iv, char *fmt,...)
{
  int i, count;
  integer lstr;
  va_list ap;
  char s_buf[1024];
  va_start (ap, fmt);
  C2F (xscion) (&i);
  if (i == 0)
    {
      count = vfprintf (stdout, fmt, ap);
    }
  else
    {
      count = vsprintf (s_buf, fmt, ap);
      TextPutS (&textwin, s_buf);
    }
  va_end (ap);
  return count;
}



size_t 
MyFWrite (const void *ptr, size_t size, size_t n, FILE * file)
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

size_t 
MyFRead (void *ptr, size_t size, size_t n, FILE * file)
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


/* I/O Function for scilab : this function are used when Xscilab is on */

extern void Xputchar ();

void 
Xputchar (c)
     char c;
{
  MyPutCh ((int) c);
}

void 
Xputstring (str, n)
     char *str;
     int n;
{
  int i;
  for (i = 0; i < n; i++)
    Xputchar (str[i]);
  TextMessage ();
}

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

#define MORESTR "[More (y or n ) ?] "

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

/* I/O Function for C routines : test for xscion */

void 
Scisncr (str)
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

int 
ReadKey (void)
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
#ifdef __GNUC__
	  if (irBuffer.Event.KeyEvent.AsciiChar != '\0')
	    {
#else
	  if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
	    {
#endif
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


int 
WriteStr (char *s, FILE * file)
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

int 
WriteKey (char c, FILE * file)
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
