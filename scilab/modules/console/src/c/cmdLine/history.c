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
#include <unistd.h>
#include <term.h>
#include <termios.h>
#include <wchar.h>
#include "cap_func.h"
#include "goto_func.h"
#include "aff_prompt.h"
#include "reader.h"
#include "history.h"
#include "HistoryManager.h"
#include "MALLOC.h"
#include "charEncoding.h"

/* Get the prev command line */
int previousCmd(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int promptSize;

    char *multiByteString = NULL;

    /* Go the beginning of the current edited line then clearn the screen from */
    while (*cursorLocation)
    {
        gotoLeft(*commandLine, cursorLocation);
    }
    capStr("up");               /* TODO: rename this function */
    capStr("do");
    capStr("cd");
    /* Get the new command line then display it */
    promptSize = getPrompt(WRT_PRT);
    multiByteString = getPreviousLineInScilabHistory();
    if (multiByteString != NULL)
    {
        *commandLine = to_wide_string(multiByteString);
        FREE(multiByteString);
    }
    *cursorLocation = wcslen(*commandLine);
    printf("%ls", *commandLine);
    /*
     * if the last character is on the last column of the window,
     * put the cursor on the first column of the next line.
     *
     * Must be done, else the cursor disappear and bug.
     */
    if (!((*cursorLocation + promptSize) % tgetnum("co")))
    {
        capStr("do");
    }
    return 0;
}

/* Get the next command line */
int nextCmd(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int promptSize;

    char *multiByteString = NULL;

    /* Go the beginning of the current edited line then clearn the screen from */
    while (*cursorLocation)
    {
        gotoLeft(*commandLine, cursorLocation);
    }
    capStr("up");
    capStr("do");
    capStr("cd");
    /* Get the new command line then display it */
    promptSize = getPrompt(WRT_PRT);
    multiByteString = getNextLineInScilabHistory();
    if (multiByteString != NULL)
    {
        *commandLine = to_wide_string(multiByteString);
        FREE(multiByteString);
    }
    *cursorLocation = wcslen(*commandLine);
    printf("%ls", *commandLine);
    /*
     * if the last character is on the last column of the window,
     * put the cursor on the first column of the next line.
     *
     * Must be done, else the cursor disappear and bug.
     */
    if (!((*cursorLocation + promptSize) % tgetnum("co")))
    {
        capStr("do");
    }
    return 0;
}
