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

#include <string.h>
#include <wctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <curses.h>
#include "cliPrompt.h"
#include "prompt.h"
#include "cliDisplayManagement.h"

/*
 * The function shows the current prompt
 * If none are set, function set it to the basic prompt SCIPROMPT ("-->") using wchar.
 * If Argument pass is WRITE_PROMPT (1), it write prompt (NOWRITE_PROMPT (-1) not to write prompt)
 * Function return size of the prompt.
 */
int printPrompt(int token)
{
    char * prompt = (char*)malloc(sizeof(char) * (PROMPT_SIZE_MAX + 1));
    char * tmpPrompt = GetTemporaryPrompt();
    int promptLen;

    /* Retrieve the prompt. It can be different if the pause mode is enabled */
    GetCurrentPrompt(prompt);

    if (token == WRITE_PROMPT)
    {
        setCharDisplay(DISP_DEFAULT);
        if (tmpPrompt)
        {
            printf("%s", tmpPrompt);
        }
        else
        {
            printf("%s", prompt);
        }
        setCharDisplay(DISP_LAST_SET);
        fflush(stdout);
    }

    promptLen = strlen(prompt);
    free(prompt);
    return promptLen;
}

