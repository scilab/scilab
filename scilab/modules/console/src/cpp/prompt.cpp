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
#include <string>

extern "C"
{
#include "prompt.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "localization.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "BOOL.h"
}
/*------------------------------------------------------------------------*/
static std::string Sci_Prompt;
static std::string temporaryPrompt;
static BOOL dispWarningLevelPrompt = TRUE;
/*------------------------------------------------------------------------*/
/* setprlev : set the current prompt string */
/*------------------------------------------------------------------------*/
void setPreviousLevel(int pause)
{
    //debugger prompt first !
    if (isEnableDebug())
    {
        if (isDebugInterrupted())
        {
            Sci_Prompt = SCIPROMPTBREAK;
        }
        else
        {
            Sci_Prompt = SCIPROMPTDEBUG;
        }
    }
    else if (pause == 0)
    {
        if (temporaryPrompt.empty() == false)
        {
            Sci_Prompt = temporaryPrompt;
            ClearTemporaryPrompt();
        }
        else
        {
            Sci_Prompt = SCIPROMPT;
        }
    }
    else if (pause > 0)
    {
        if (dispWarningLevelPrompt)
        {
            if (getWarningMode())
            {
                sciprint(_("Type '%s' or '%s' to return to standard level prompt.\n\n"), "resume", "abort");
                dispWarningLevelPrompt = FALSE;
            }
        }
        char t[50];

        sprintf(t, SCIPROMPT_INTERRUPT, pause);
        Sci_Prompt = t;
        // bug 5513
        // when we change prompt to a pause level, we change also temp. prompt
        //SetTemporaryPrompt(Sci_Prompt);
    }
    else
    {
        Sci_Prompt = SCIPROMPT_PAUSE;
        // bug 5513
        // when we change prompt to halt level, we change also temp. prompt
        SetTemporaryPrompt(Sci_Prompt.data());
    }
}
/*------------------------------------------------------------------------*/
const char* GetCurrentPrompt()
{
    return Sci_Prompt.data();
}
/*------------------------------------------------------------------------*/
void SetTemporaryPrompt(const char* tempPrompt)
{
    temporaryPrompt = tempPrompt;
}
/*------------------------------------------------------------------------*/
const char* GetTemporaryPrompt(void)
{
    return temporaryPrompt.data();
}
/*------------------------------------------------------------------------*/
void ClearTemporaryPrompt(void)
{
    temporaryPrompt = "";
}
/*------------------------------------------------------------------------*/
