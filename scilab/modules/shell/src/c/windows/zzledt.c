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
#include "sci_mem_alloc.h"
#include "scilabmode.h"
#include "ConsoleRead.h"
#include "zzledt.h"
#include "prompt.h"
#include "SetConsolePrompt.h"
#include "TermReadAndProcess.h"
/*--------------------------------------------------------------------------*/
void C2F (zzledt) (char *buffer, int *buf_size, int *len_line, int *eof, int* interrupt, int *modex, long int dummy1)
{
	int i = 0;
	char *line = NULL;

	if (getScilabMode() == SCILAB_STD)
	{
		static char save_prompt[10];

		GetCurrentPrompt(save_prompt);

		SetConsolePrompt(save_prompt);
		line = ConsoleRead();
		SetConsolePrompt(save_prompt);
	}
	else
	{
		line = TermReadAndProcess();
	}

	if (line)
	{
		strncpy (buffer, line, *buf_size);
		*len_line = (int)strlen (buffer);
		FREE(line);
		line = NULL;
	}

	*eof = FALSE;
}
/*--------------------------------------------------------------------------*/

