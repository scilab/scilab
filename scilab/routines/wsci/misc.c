/* misc.c */
/* From GNUPLOT - util.c */
/*
 * Copyright (C) 1986 - 1993   Thomas Williams, Colin Kelley
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
 *   Original Software:
 *     Thomas Williams,  Colin Kelley.
 *   Gnuplot 2.0 additions:
 *       Russell Lang, Dave Kotz, John Campbell.
 *   Gnuplot 3.0 additions:
 *       Gershon Elber and many others.
 *   Scilab-2.3 : changes for scilab 
 *       (1997) : Jean-Philippe Chancelier 
 */


#include <ctype.h>
#include <setjmp.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>		/* for malloc */
#include <string.h>
#include <math.h>

//#include "plot.h"
#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
  #include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"

#include "../os_specific/win_mem_alloc.h" /* MALLOC */

extern char input_line[];
extern jmp_buf env;		/* from plot.c */
/****************************************************************
*alloc:
* allocate memory 
* This is a protected version of malloc. It causes an int_error 
* if there is not enough memory. If message is NULL, we 
* allow NULL return. Otherwise, we handle the error, using the
* message to create the int_error string. Note cp/sp_extend uses realloc,
* so it depends on this using malloc().
*****************************************************************/

char * alloc (unsigned long size, char *message)
/* unsigned long size;     # of bytes */
/* char *message;           description of what is being allocated */
{
	char *p;			/* the new allocation */
	char errbuf[100];		/* error message string */
	p = MALLOC ((size_t) size);	/* try again */
	if (p == (char *) NULL)
	{
		/* really out of memory */
		if (message != NULL)
		{
			(void) sprintf (errbuf, MSG_ERROR75, message);
			int_error (errbuf, NO_CARET);
			/* NOTREACHED */
		}
		/* else we return NULL */
	}
	return (p);
}
/******************************************************
 * find char c in string str; return p such that str[p]==c;
 * if c not in str then p=strlen(str)
 *****************************************************/

int instring (char *str, char c)
{
  int pos = 0;
  while (str != NULL && *str != '\0' && c != *str)
    {
      str++;
      pos++;
    }
  return (pos);
}



void 
int_error (char *str, int t_num)
{
  /* reprint line if screen has been written to */
  if (t_num != NO_CARET)
    {				/* put caret under error */
      sciprint ("\n%s%s\n", "-->", input_line);
    }
  sciprint ("\t%s\n\n", str);
  longjmp (env, TRUE);		/* bail out to command line */
}

/********************************************/
/* Lower-case the given string (DFK) */
/* Done in place. */
/********************************************/

void 
lower_case (char *s)
{
  register char *p = s;
  while (*p != '\0')
    {
      if (isupper ((int) *p))
	*p = tolower (*p);
      p++;
    }
}

/********************************************/
/* Squash spaces in the given string (DFK) */
/* That is, reduce all multiple white-space chars to single spaces */
/* Done in place. */
/********************************************/

void 
squash_spaces (char *s)
{
  register char *r = s;		/* reading point */
  register char *w = s;		/* writing point */
  int space = FALSE;		/* TRUE if we've already copied a space */

  for (w = r = s; *r != '\0'; r++)
    {
      if (isspace ((int) *r))
	{
	  /* white space; only copy if we haven't just copied a space */
	  if (!space)
	    {
	      space = TRUE;
	      *w++ = ' ';
	    }			/* else ignore multiple spaces */
	}
      else
	{
	  /* non-space character; copy it and clear flag */
	  *w++ = *r;
	  space = FALSE;
	}
    }
  *w = '\0';			/* null terminate string */
}
