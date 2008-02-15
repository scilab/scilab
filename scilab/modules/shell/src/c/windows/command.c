/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Scilab 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "sciprint.h"
#include "localization.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "prompt.h"
#include "../../core/src/c/flags.h"
#include "HistoryManager.h"
#include "readline_nw.h"
/*--------------------------------------------------------------------------*/
#define NO_CARET (-1)
/*--------------------------------------------------------------------------*/
/* input data, parsing variables */
char input_line[MAX_LINE_LEN + 1] = "";
/*--------------------------------------------------------------------------*/
static char *rlgets_nw (char *s, int n, char *prompt, int interrupt);
static void int_error (char *str, int t_num);
/*--------------------------------------------------------------------------*/
int read_line (char *prompt, int interrupt)
{
	int start = 0, ilen = 0;
	int last = 0;
	char *p = NULL;

	ilen = MAX_LINE_LEN - start - 1;
	input_line[start] = ilen > 126 ? 126 : ilen;
	input_line[start + 2] = 0;

	rlgets_nw (&(input_line[start+2]), ilen, prompt, interrupt);

	if (input_line[start + 2] == -2)
	{
		/* dynamic menu canceled read SS */
		return (-1);
	}
	if ((p = strchr (&(input_line[start + 2]), '\r')) != NULL) *p = NULL;
	if ((p = strchr (&(input_line[start + 2]), '\n')) != NULL) *p = NULL;

	input_line[start + 1] = (char)strlen (&(input_line[start + 2]));

	if (input_line[start + 2] == 26 || input_line[start + 2] == -1)
	{
		/* end-of-file or end reached in pipe */
		input_line[start] = '\0';

		if (start <= 0)		
		{
			/* don't quit yet - process what we have */
			sciprint ("\n");
			/* exit scilab */
			return (1);		
		}
	}
	else
	{
		/* normal line input */
		register int i = start;

		/* move everything down two characters */
		while ((input_line[i] = input_line[i + 2]) != (char) NULL) i++;

		last = (int)strlen (input_line) - 1;
		if (last < 0) last = 0;	
		if (last + 1 >= MAX_LINE_LEN) int_error (_("Input line too long"), NO_CARET);
	}
	return (0);
}
/*--------------------------------------------------------------------------*/
/* get line form console */
static char * rlgets_nw (char *s, int n, char *prompt, int interrupt)
{
  static char *line = (char *) NULL;

  /* If we already have a line, first FREE it */
  if (line != (char *) NULL)   FREE (line);

  line = readline_nw (prompt, interrupt);

  /* If it's not an EOF */
  if (line)
  {
      /* -1 is added for eos ( end of input when using pipes ) */
      if (*line>=0) appendLineToScilabHistory(line);
      strncpy (s, line, n);
      return s;
  }
  return line;
}
/*--------------------------------------------------------------------------*/
static void int_error (char *str, int t_num)
{
	/* reprint line if screen has been written to */

	if (t_num != NO_CARET)
	{				
		/* put caret under error */
		sciprint ("\n%s%s\n", SCIPROMPT, input_line);
	}
	sciprint ("\t%s\n\n", str);
}
/*--------------------------------------------------------------------------*/
