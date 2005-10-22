
/*
 * command.c : 
 * (1997) : Jean-Philippe Chancelier 
 * (2004) : Allan CORNET
 * 
 */

#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "wcommon.h"
//#include "plot.h"
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */

#ifndef STRICT
#define STRICT
#endif

struct hist
{
    		char *line;
    		struct hist *prev;
    		struct hist *next;
};

extern void AddHistory (char *line);
extern struct hist * SearchBackwardInHistory(char *line);
extern BOOL NewSearchInHistory;

extern GW graphwin;		/* graphic window */
extern TW textwin;
extern jmp_buf env;		/* from plot.c */

static char *rlgets (char *s, int n, char *prompt, int interrupt);
static char *rlgets_nw (char *s, int n, char *prompt, int interrupt);

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

static char * rlgets (char *s, int n, char *prompt, int interrupt)
{
  static char *line = (char *) NULL;
  
  /* If we already have a line, first FREE it */
  if (line != (char *) NULL)
    FREE (line);

  line = readline_win (prompt,interrupt);
  NewSearchInHistory=TRUE;	
  /* If it's not an EOF */
  if (line)
    {
      if ( (*line>=0) && (strlen(line)>0) )AddHistory (line);
      strncpy (s, line, n);
      return s;
    }
  
  return line;
}

/**********************************************
 * Used in read_line 
 * version with readline and no textwindow scilab -nw *
 **********************************************/

static char * rlgets_nw (char *s, int n, char *prompt, int interrupt)
{
  static char *line = (char *) NULL;

  /* If we already have a line, first FREE it */
  if (line != (char *) NULL)   FREE (line);
  line = readline_nw (prompt, interrupt);
  NewSearchInHistory=TRUE;	
  /* If it's not an EOF */
  if (line)
    {
      /* -1 is added for eos ( end of input when using pipes ) */
      if (*line>=0)
	AddHistory (line);
      strncpy (s, line, n);
      return s;
    }
  return line;
}

typedef char *(*RLFUNC) (char *, int, char *, int);
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

int read_line (char *prompt, int interrupt)
{
  
  int start = 0, ilen = 0;
  int last;
  char *p;
  ilen = MAX_LINE_LEN - start - 1;
  input_line[start] = ilen > 126 ? 126 : ilen;
  input_line[start + 2] = 0;
  
  (void) (*rlgets_def) (&(input_line[start+2]), ilen, prompt, interrupt);

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
	int_error (MSG_ERROR68, NO_CARET);
    }
    
  return (0);
  
}


