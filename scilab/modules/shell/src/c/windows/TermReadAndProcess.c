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
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#define strdup _strdup
/*--------------------------------------------------------------------------*/
char * TermReadAndProcess(void)
{
	static char save_prompt[10];
	char *OEMline = NULL;
	char *returnedline = NULL;

    if (GetTemporaryPrompt()!=NULL) /* Input function is used */
    {
		OEMline = readline_nw(GetTemporaryPrompt());
        ClearTemporaryPrompt();
	}
	else
	{
		GetCurrentPrompt(save_prompt);
		OEMline = readline_nw (save_prompt);
	}
        
	if (OEMline)
	{
		char *line = NULL;
		char *UTF8line = NULL;

		line = (char*)MALLOC(sizeof(char)*(strlen(OEMline)+1));
		if (line)
		{
			strcpy(line,"");
			OemToChar(OEMline,line);

			appendLineToScilabHistory(OEMline);

			UTF8line = ANSIToUTF8(line);
			if (UTF8line)
			{
				returnedline = strdup(UTF8line);
				FREE(UTF8line);
				UTF8line = NULL;
			}

			FREE(line);
			line = NULL;
		}

		FREE(OEMline);
		OEMline = NULL;
	}
	strcpy(save_prompt,"");
	return returnedline;
}
/*--------------------------------------------------------------------------*/
