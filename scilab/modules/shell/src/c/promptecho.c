/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include "stack-def.h"
#include "promptecho.h"
#include "basout.h"
#include "MALLOC.h"
#include "prompt.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
/*--------------------------------------------------------------------------*/ 
int C2F(promptecho)(int *lunit, char *string, int *strl,int string_len)
{
	char *promptstr = NULL;
	char *buffstr =  NULL;
	int lenbuffstr = 0;

	if (GetTemporaryPrompt())
	{
		promptstr = strdup(GetTemporaryPrompt());
	}
	else
	{
		promptstr = (char*)MALLOC(sizeof(char)*(PROMPT_SIZE_MAX+1));
		GetCurrentPrompt(promptstr);
	}

	/* concates prompt with input command line */
	if (promptstr)
	{
		lenbuffstr = (int) ( strlen(string) + strlen(promptstr) );
		buffstr = (char*) MALLOC(sizeof(char)* (lenbuffstr + 1));
		if (buffstr)
		{
			strcpy(buffstr,promptstr);
			strcat(buffstr,string);
		}
		FREE(promptstr);
		promptstr = NULL;
	}

	/* displays command line with prompt */
	if (buffstr)
	{
		int io = 0;
		C2F(basout)(&io, lunit, buffstr,lenbuffstr);
		FREE(buffstr);
		buffstr = NULL;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/ 
