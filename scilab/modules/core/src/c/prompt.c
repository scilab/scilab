/*------------------------------------------------------------------------*/
/*    Copyright (C) Inria */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "prompt.h"
#include "message_scilab.h"
#include "sciprint.h"
#include "warningmode.h"
/*------------------------------------------------------------------------*/
static char Sci_Prompt[10];
static BOOL dispWarningLevelPrompt=TRUE;
/*------------------------------------------------------------------------*/
/* setprlev : set the current prompt string */
/*------------------------------------------------------------------------*/
void C2F(setprlev)( int *pause)
{
	if ( *pause == 0 ) sprintf(Sci_Prompt,SCIPROMPT);
	else if ( *pause > 0 )
	{
		if (dispWarningLevelPrompt)
		{
			if (getWarningMode())
			{
				message_scilab("Type 'resume' or 'abort' to return to standard level prompt.");
				sciprint("\n");
				dispWarningLevelPrompt=FALSE;
			}
		}
		sprintf(Sci_Prompt,SCIPROMPT_INTERRUPT,*pause);
	}
	else sprintf(Sci_Prompt,SCIPROMPT_PAUSE);
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
