/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Karim Mamode
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

