/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
#include <stdio.h>
#include <string.h>
#include "prompt.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "sci_malloc.h"
#include "os_string.h"
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
    //debugger prompt first !
    if (isEnableDebug())
    {
        if (isDebugInterrupted())
        {
            sprintf(Sci_Prompt, SCIPROMPTBREAK);
        }
        else
        {
            sprintf(Sci_Prompt, SCIPROMPTDEBUG);
        }
    }
    else if ( *pause == 0 )
    {
        if (temporaryPrompt != NULL)
        {
            strcpy(Sci_Prompt, temporaryPrompt);
            ClearTemporaryPrompt();
        }
        else
        {
            sprintf(Sci_Prompt, SCIPROMPT);
        }
    }
    else if ( *pause > 0 )
    {
        if (dispWarningLevelPrompt)
        {
            if (getWarningMode())
            {
                sciprint(_("Type '%s' or '%s' to return to standard level prompt.\n\n"), "resume", "abort");
                dispWarningLevelPrompt = FALSE;
            }
        }
        sprintf(Sci_Prompt, SCIPROMPT_INTERRUPT, *pause);
        // bug 5513
        // when we change prompt to a pause level, we change also temp. prompt
        //SetTemporaryPrompt(Sci_Prompt);
    }
    else
    {
        sprintf(Sci_Prompt, SCIPROMPT_PAUSE);
        // bug 5513
        // when we change prompt to halt level, we change also temp. prompt
        SetTemporaryPrompt(Sci_Prompt);
    }
}
/*------------------------------------------------------------------------*/
void GetCurrentPrompt(char *CurrentPrompt)
{
    if (CurrentPrompt)
    {
        strcpy(CurrentPrompt, Sci_Prompt);
    }
}
/*------------------------------------------------------------------------*/
void SetTemporaryPrompt(const char *tempPrompt)
{
    ClearTemporaryPrompt();
    temporaryPrompt = os_strdup(tempPrompt);
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
