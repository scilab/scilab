/*
 * command.c : 
 * (1997) : Jean-Philippe Chancelier 
 * 
 */

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>*/
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#if !(defined __CYGWIN32__) && !(defined __ABSC__)
#include <direct.h>		/* for _chdrive() */
#endif

#include <time.h>		/* kludge to provide sleep() */

/*#include "wgnuplib.h"*/
#include "wcommon.h"
#include "plot.h"

extern GW graphwin;		/* graphic window */
extern TW textwin;
extern jmp_buf env;		/* from plot.c */


static char *rlgets (char *s, int n, char *prompt);
static char *rlgets_nw (char *s, int n, char *prompt);

/* input data, parsing variables */

char input_line[MAX_LINE_LEN + 1] = "";
int inline_num = 0;		/* input line number */

/**********************************************
 * Support for input, shell,printer for win32 
 **********************************************/

/**********************************************
 * Used in read_line 
 * version with readline and a textwindow 
 **********************************************/

static char *
rlgets (char *s, int n, char *prompt)
{
  static char *line = (char *) NULL;

  /* If we already have a line, first free it */
  if (line != (char *) NULL)
    free (line);

  line = readline_win (prompt);

  /* If it's not an EOF */
  if (line)
    {
      if (*line>=0)
	add_history_win (line);
      strncpy (s, line, n);
      return s;
    }
  return line;
}

/**********************************************
 * Used in read_line 
 * version with readline and no textwindow scilab -nw *
 **********************************************/

static char *
rlgets_nw (char *s, int n, char *prompt)
{
  static char *line = (char *) NULL;

  /* If we already have a line, first free it */
  if (line != (char *) NULL)
    free (line);
  line = readline_nw (prompt);
  /* If it's not an EOF */
  if (line)
    {
      /* -1 is added for eos ( end of input when using pipes ) */
      if (*line>=0)
	add_history_nw (line);
      strncpy (s, line, n);
      return s;
    }
  return line;
}

typedef char *(*RLFUNC) (char *, int, char *);
RLFUNC rlgets_def = rlgets_nw;

void 
switch_rlgets (int i)
{
  if (i == 1)
    rlgets_def = rlgets;
  else
    rlgets_def = rlgets_nw;
}

/**********************************************
 * reads a scilab line with rlgets or rlgets_nw 
 * according to current value of rlgets_def 
 **********************************************/

int 
read_line (char *prompt)
{

  int start = 0, ilen = 0;
  int last;
  char *p;
  ilen = MAX_LINE_LEN - start - 1;
  input_line[start] = ilen > 126 ? 126 : ilen;
  input_line[start + 2] = 0;
  (void) (*rlgets_def) (&(input_line[start + 2]), ilen, prompt);
  if (input_line[start + 2] == -2) /* dynamic menu canceled read SS */
    {
      return (-1);
    }
  if ((p = strchr (&(input_line[start + 2]), '\r')) != NULL)
    *p = 0;
  if ((p = strchr (&(input_line[start + 2]), '\n')) != NULL)
    *p = 0;
  input_line[start + 1] = strlen (&(input_line[start + 2]));

  if (input_line[start + 2] == 26 || input_line[start + 2] == -1)
    {
      /* XXX 26 XXXX */
      /* end-of-file or end reached in pipe */
      input_line[start] = '\0';
      inline_num++;
      if (start <= 0)		/* don't quit yet - process what we have */
	{
	  sciprint ("\n");
	  return (1);		/* exit scilab */
	}
    }
  else
    {
      /* normal line input */
      register int i = start;
      while ((input_line[i] = input_line[i + 2]) != (char) NULL)
	i++;			/* yuck!  move everything down two characters */

      inline_num++;
      last = strlen (input_line) - 1;
      if (last < 0)
	last = 0;		/* stop UAE in Windows */
      if (last + 1 >= MAX_LINE_LEN)
	int_error ("Input line too long", NO_CARET);
    }
  return (0);
}

/**********************************************
 * a shell function 
 * XXXX should use /bin/sh if we can find it 
 **********************************************/

void 
do_shell ()
{
  register char *comspec;
  if ((comspec = getenv ("COMSPEC")) == (char *) NULL)
    comspec = "\\command.com";
  if (WinExec (comspec, SW_SHOWNORMAL) <= 32)
    os_error ("unable to spawn shell", NO_CARET);
}

/**********************************************
 * A system function  (Windows style )
 * see also the system function.
 **********************************************/

void 
do_system ()
{
  if (winsystem (input_line + 1, 0))
    os_error ("system() failed", NO_CARET);
}


void 
do_system1 ()
{
  sciprint ("%d \r\n", system (input_line + 1));
}



/* there is a system like call on MS Windows but it is a bit difficult to 
   use, so we will invoke the command interpreter and use it to execute the 
   commands */

/** #define WINEXECDEBUG **/

int 
winsystem (char *s, int flag)
{
  LPSTR comspec, execstr, p;
  int sw_sci_flag;
  /* get COMSPEC environment variable */
  char envbuf[81];
  if (flag == 1)
    sw_sci_flag = SW_SHOWNORMAL;
  else
    sw_sci_flag = SW_HIDE;

  GetEnvironmentVariable ("COMSPEC", envbuf, 80);
  if (*envbuf == '\0')
    comspec = "\\command.com";
  else
    comspec = envbuf;
  /* if the command is blank we must use command.com */
  p = s;
  while ((*p == ' ') || (*p == '\n') || (*p == '\r'))
    p++;
  if (*p == '\0')
    {
#ifdef WINEXECDEBUG
      sciprint ("TestMessage0 : WinExec de %s\r\n", comspec);
#endif
      WinExec (comspec, SW_SHOWNORMAL);
    }
  else
    {
      /* attempt to run the windows/dos program via windows */
      int res;
      execstr = (char *) malloc (strlen (s) + strlen (comspec) + 6);
      if (execstr == NULL)
	{
	  sciprint ("Running out of memory\r\n");
	  return 1;
	}
      if (s[0] == '/' && s[1] == '/' && s[3] == '/')
	{
	  sprintf (execstr, "%c:%s", s[2], s + 3);
#ifdef WINEXECDEBUG
	  sciprint ("TestMessage1 : WinExec de %s\r\n", execstr);
#endif
	  res = WinExec (execstr, sw_sci_flag);
	}
      else
	{
	  sprintf (execstr, "%s", s);
#ifdef WINEXECDEBUG
	  sciprint ("TestMessage2 : WinExec de %s\r\n", execstr);
#endif
	  res = WinExec (execstr, sw_sci_flag);
	}
      if (res <= 31)
	{
	  /* attempt to run it as a dos program from command line */
	  if (s[0] == '/' && s[1] == '/' && s[3] == '/')
	    {
	      sprintf (execstr, "%s /c %c:%s", comspec, s[2], s + 3);
	    }
	  else
	    {
	      sprintf (execstr, "%s /c %s", comspec, s);
	    }
#ifdef WINEXECDEBUG
	  sciprint ("TestMessage3 : WinExec de %s\r\n", execstr);
#endif
	  res = WinExec (execstr, sw_sci_flag);
	  if (res <= 31)
	    sciprint ("WinExec of %s failed \r\n", execstr);
	}
      free (execstr);
    }
  /* regardless of the reality return OK - the consequences of */
  /* failure include shutting down Windows */
  return (0);			/* success */
}

/** pas franchement utile **/

int 
MyWinExec (char *str, int sw_sci_flag)
{
  STARTUPINFO start;
  SECURITY_ATTRIBUTES sec_attrs;
  PROCESS_INFORMATION child;
  int wait_for_child = TRUE;
  DWORD ret_code = 0;
  memset (&start, 0, sizeof (start));
  start.cb = sizeof (start);
  start.dwFlags = STARTF_USESHOWWINDOW;
  start.wShowWindow = SW_HIDE;
/** start.wShowWindow = SW_SHOWMINIMIZED; **/
  sec_attrs.nLength = sizeof (sec_attrs);
  sec_attrs.lpSecurityDescriptor = NULL;
  sec_attrs.bInheritHandle = FALSE;
  if (CreateProcess (NULL, str, &sec_attrs, NULL, TRUE, 0,
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
  if ((int) ret_code == 0)
    return 32;
  else
    return 0;
error:
  sciprint (" Could not exec %s\r\n", str);
  return 32;
}
