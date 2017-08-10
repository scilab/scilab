/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
