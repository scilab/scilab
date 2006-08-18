/*------------------------------------------------------------------------*/
/*    Copyright (C) Inria */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../machine.h"
/*------------------------------------------------------------------------*/
static char Sci_Prompt[10];
/*------------------------------------------------------------------------*/
/* setprlev : set the current prompt string */
/*------------------------------------------------------------------------*/
void C2F(setprlev)( int *pause)
{
	if ( *pause == 0 ) 
		sprintf(Sci_Prompt,"-->");
	else if ( *pause > 0 )
		sprintf(Sci_Prompt,"-%d->",*pause);
	else
		sprintf(Sci_Prompt,">>");
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
