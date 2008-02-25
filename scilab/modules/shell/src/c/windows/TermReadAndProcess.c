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
#include <string.h>

#include "TermReadAndProcess.h"
#include "command.h"
#include "prompt.h"
/*--------------------------------------------------------------------------*/
extern char input_line[MAX_LINE_LEN + 1]; /* defined in command.c */

char * TermReadAndProcess(void)
{
	static char save_prompt[10];
	char *line = NULL;
	int i = 0;
	int interrupt = 0;

	GetCurrentPrompt(save_prompt);
	i = read_line (save_prompt,interrupt);
	line =  strdup(input_line);
	return line;
	
}
/*--------------------------------------------------------------------------*/
