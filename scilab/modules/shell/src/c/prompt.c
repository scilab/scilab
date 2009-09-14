/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include "prompt.h"
#include "sciprint.h"
#include "warningmode.h"
#include "localization.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
#include "BOOL.h"
/*------------------------------------------------------------------------*/
static char Sci_Prompt[PROMPT_SIZE_MAX];
static BOOL dispWarningLevelPrompt = TRUE;
static char *temporaryPrompt = NULL;
/*------------------------------------------------------------------------*/
/* setprlev : set the current prompt string */
/*------------------------------------------------------------------------*/
void C2F(setprlev)( int *pause)
{
	if ( *pause == 0 ) 
	{
		sprintf(Sci_Prompt,SCIPROMPT);
	}
	else if ( *pause > 0 )
	{
		if (dispWarningLevelPrompt)
		{
			if (getWarningMode())
			{
				sciprint(_("Type '%s' or '%s' to return to standard level prompt.\n\n"),"resume","abort");
				dispWarningLevelPrompt = FALSE;
			}
		}
		sprintf(Sci_Prompt,SCIPROMPT_INTERRUPT,*pause);
	}
	else 
	{
		sprintf(Sci_Prompt,SCIPROMPT_PAUSE);
	}
}
/*------------------------------------------------------------------------*/
void GetCurrentPrompt(char *CurrentPrompt)
{
	if (CurrentPrompt)
	{
		strcpy(CurrentPrompt,Sci_Prompt);
	}
}
/*------------------------------------------------------------------------*/
void SetTemporaryPrompt(char *tempPrompt)
{
	if (temporaryPrompt) 
	{
		FREE(temporaryPrompt);
		temporaryPrompt = NULL;
	}
	temporaryPrompt = strdup(tempPrompt);
}
/*------------------------------------------------------------------------*/
char *GetTemporaryPrompt(void)
{
  return temporaryPrompt;
}
/*------------------------------------------------------------------------*/
void ClearTemporaryPrompt(void)
{
	if (temporaryPrompt)
	{
		FREE(temporaryPrompt);
		temporaryPrompt = NULL;
	}
}
/*------------------------------------------------------------------------*/
