/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
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
#include <string.h>
#include "machine.h"
#include "command.h"
#include "../../core/src/c/flags.h"
#include "sci_mem_alloc.h"
#include "scilabmode.h"
#include "ConsoleRead.h"
#include "zzledt.h"
#include "prompt.h"
#include "SetConsolePrompt.h"
/*--------------------------------------------------------------------------*/
static char save_prompt[10];
/*--------------------------------------------------------------------------*/
extern char input_line[MAX_LINE_LEN + 1]; /* defined in command.c */
/*--------------------------------------------------------------------------*/
void C2F (zzledt) (char *buffer, int *buf_size, int *len_line, int *eof, int* interrupt, int *modex, long int dummy1)
{
  int i = 0;
  char *line = NULL;

  GetCurrentPrompt(save_prompt);

  set_is_reading (TRUE);
  if (getScilabMode() == SCILAB_STD)
  {
	  SetConsolePrompt(save_prompt);
	  i = 0;
	  line = ConsoleRead();

	  if (line)
	  {
		  strcpy(input_line,line);  
		  *len_line = (int)strlen(line);
		  FREE(line); /* malloc in ConsoleRead */
		  line = NULL;
	  }
	  SetConsolePrompt(save_prompt);
  }
  else
  {
	i = read_line (save_prompt,*interrupt);
  }

  if (i==-1) 
  { 
	/* dynamic menu canceled read SS*/
    *len_line = 0;
    *eof = -1;
    return;
  }
  strncpy (buffer, input_line, *buf_size);
  *len_line = (int)strlen (buffer);

  *eof = (i == 1) ? TRUE : FALSE;
  set_is_reading (FALSE);
  return;
}
/*--------------------------------------------------------------------------*/
