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
#include "MALLOC.h"
#include "prompt.h"
#include "TermConsole.h"
/*--------------------------------------------------------------------------*/
char * getCmdLine(void)
{
	char *line = NULL;
	static char save_prompt[PROMPT_SIZE_MAX];

	if (GetTemporaryPrompt() != NULL) /* Input function is used */
	{
		line = TerminalGetString(GetTemporaryPrompt());
	}
	else
	{
		GetCurrentPrompt(save_prompt);
		line = TerminalGetString(save_prompt);
	}
	strcpy(save_prompt,"");
	return line;
}
/*--------------------------------------------------------------------------*/
