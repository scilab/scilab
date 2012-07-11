/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include "cliDisplayManagement.h"

static BOOL cliColor = TRUE;

/* Set color and other settings about character display */
void setCharDisplay(char *setting)
{
    static char *currentSetting = NULL;

    static char *lastSetting = NULL;

    if (!getCLIColor())
    {
        return;
    }

    /* If NULL is passed, Previous settings are set. */
    if (setting == NULL)
    {
        if (lastSetting != NULL)
        {
            setting = lastSetting;
        }
        else                    /* currentSetting should never be NULL */
        {
            setting = currentSetting;
        }
    }
    /* Print the setting to apply */
    printf("\033[%sm", setting);
    /* The save preious and current settings */
    lastSetting = currentSetting;
    currentSetting = setting;
    fflush(stdout);
}

/* Change Characters color */
void setForegroundColor(int color)
{
    char charColor[3];

    charColor[0] = SET_CONSOLE_FOREGROUND;
    charColor[1] = color + '0';
    charColor[2] = '\0';
    setCharDisplay(charColor);
}

/* Change background color */
void setBackgroundColor(int color)
{
    char charColor[3];

    charColor[0] = SET_CONSOLE_BACKGROUND;
    charColor[1] = color + '0';
    charColor[2] = '\0';
    setCharDisplay(charColor);
}

BOOL getCLIColor()
{
    return cliColor;
}

void setCLIColor(BOOL CLIColor)
{
    cliColor = CLIColor;
}
