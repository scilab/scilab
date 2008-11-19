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
#include "HistoryManager.h"
#include "TermReadAndProcess.h"
#include "MALLOC.h"
#include "prompt.h"
#include "readline_nw.h"
#include "strdup_Windows.h"
/*--------------------------------------------------------------------------*/
static char *returnedline = NULL;
/*--------------------------------------------------------------------------*/
char * TermReadAndProcess(void)
{
	static char save_prompt[10];
	char* OEMChars = NULL;

	/* free previous line */
	if (returnedline) {FREE(returnedline);returnedline = NULL;}

    if (GetTemporaryPrompt()!=NULL) /* Input function is used */
    {
		OEMChars = readline_nw(GetTemporaryPrompt());
        ClearTemporaryPrompt();
	}
	else
	{
		GetCurrentPrompt(save_prompt);
		OEMChars = readline_nw (save_prompt);
	}

	/* NW windows term uses OEM characters */
	/* We need to convert to ANSI characters with OEMToChar */
	/* http://msdn.microsoft.com/en-us/library/ms647493(VS.85).aspx */
	returnedline = strdup(OEMChars);
	OemToChar(OEMChars, returnedline);
	if (OEMChars) {FREE(OEMChars); OEMChars = NULL;}

	strcpy(save_prompt,"");
	return returnedline;
}
/*--------------------------------------------------------------------------*/
