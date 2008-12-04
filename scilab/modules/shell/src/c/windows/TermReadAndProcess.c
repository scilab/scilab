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
static char *returnedline = NULL;
/*--------------------------------------------------------------------------*/
char * TermReadAndProcess(void)
{
	static char save_prompt[10];

	/* free previous line */
	if (returnedline) {FREE(returnedline);returnedline = NULL;}

    if (GetTemporaryPrompt() != NULL) /* Input function is used */
    {
		returnedline = TerminalGetString(GetTemporaryPrompt());
        ClearTemporaryPrompt();
	}
	else
	{
		GetCurrentPrompt(save_prompt);
		returnedline = TerminalGetString(save_prompt);
	}
	strcpy(save_prompt,"");

	return returnedline;
}
/*--------------------------------------------------------------------------*/
