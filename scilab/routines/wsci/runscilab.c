


/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */

/*
   Simple program to start Scilab with its console window hidden.
   This program is provided purely for convenience, since most users will
   use Scilab in windowing (GUI) mode, and will not want to have an extra
   console window lying around.  

   This file was adapted from runemacs.c ( emacs distribution )

 */

#ifndef WIN32
#define WIN32
#endif

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#if defined (__MSC__) || defined(__MINGW32__)
#define putenv(x) _putenv(x)
#endif

#ifdef __ABSC__
#define putenv(x) abs_putenv(x)
#endif


/********************************
 * Set up SCI, TCL_LIBRARY and TK_LIBRARY environment variables if 
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
      if ((p1 = getenv ("TCL_LIBRARY")) == (char *) 0)
	{
	  sprintf (env, "TCL_LIBRARY=%s\\tcl\\tcl8.3", p);
	  putenv (env);
	}
      if ((p1 = getenv ("TK_LIBRARY")) == (char *) 0)
	{
	  sprintf (env, "TK_LIBRARY=%s\\tcl\\tk8.3", p);
	  putenv (env);
	}
    }
}

int WINAPI
WinMain (HINSTANCE hSelf, HINSTANCE hPrev, LPSTR cmdline, int nShow)
{
  STARTUPINFO start;
  SECURITY_ATTRIBUTES sec_attrs;
  PROCESS_INFORMATION child;
  int wait_for_child = FALSE;
  DWORD ret_code = 0;
  char *new_cmdline;
  char *p;
  char modname[MAX_PATH + 1];

  if (!GetModuleFileName (NULL, modname + 1, MAX_PATH))
    goto error;
  if ((p = strrchr (modname + 1, '\\')) == NULL)
    goto error;
  *p = 0;

  SciEnv ();

  new_cmdline = malloc (MAX_PATH + strlen (cmdline) + 1);

  strcpy (new_cmdline, modname + 1);
  strcat (new_cmdline, "\\scilex.exe ");

  /* Append original arguments if any; first look for -wait as first
     argument, and apply that ourselves.  */
  if (strncmp (cmdline, "-wait", 5) == 0)
    {
      wait_for_child = TRUE;
      cmdline += 5;
    }
  strcat (new_cmdline, cmdline);
  memset (&start, 0, sizeof (start));
  start.cb = sizeof (start);
  start.dwFlags = STARTF_USESHOWWINDOW;
/** start.wShowWindow = SW_HIDE; **/
  start.wShowWindow = SW_SHOWMINIMIZED;

  sec_attrs.nLength = sizeof (sec_attrs);
  sec_attrs.lpSecurityDescriptor = NULL;
  sec_attrs.bInheritHandle = FALSE;

  if (CreateProcess (NULL, new_cmdline, &sec_attrs, NULL, TRUE, 0,
		     NULL, NULL, &start, &child))
    {
      if (wait_for_child)
	{
	  WaitForSingleObject (child.hProcess, INFINITE);
	  GetExitCodeProcess (child.hProcess, &ret_code);
	}
      CloseHandle (child.hThread);
      CloseHandle (child.hProcess);
    }
  else
    goto error;
  return (int) ret_code;

error:
  MessageBox (NULL, "Could not start Scilab.", "Error", MB_ICONSTOP);
  return 1;
}
