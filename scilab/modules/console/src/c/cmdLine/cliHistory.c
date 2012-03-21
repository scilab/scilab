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
#include "termcapManagement.h"
#include "gotoFunctions.h"
#include "cliPrompt.h"
#include "cliHistory.h"
#include "HistoryManager.h"
#include "MALLOC.h"
#include "charEncoding.h"

/* Get the prev command line */
int previousCmd(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int promptSize;

    int newSizeToAlloc = 0;

    char *multiByteHistory = NULL;

    wchar_t *wideHistory = NULL;

    /* Go the beginning of the current edited line then clearn the screen from */
    while (*cursorLocation)
    {
        gotoLeft(*commandLine, cursorLocation);
    }
    setStringCapacities("up");
    setStringCapacities("do");
    setStringCapacities("cd");
    /* Get the new command line then display it */
    promptSize = printPrompt(WRITE_PROMPT);
    multiByteHistory = getPreviousLineInScilabHistory();
    if (multiByteHistory != NULL)
    {
        wideHistory = to_wide_string(multiByteHistory);
        if (wideHistory != NULL)
        {
            /* Allocation by a multiple of 1024 */
            newSizeToAlloc = wcslen(wideHistory) / 1024 + 1;
            FREE(*commandLine);
            *commandLine = MALLOC(sizeof(**commandLine) * (newSizeToAlloc * 1024));
            if (*commandLine != NULL)
            {
                wcscpy(*commandLine, wideHistory);
            }
        }
        FREE(multiByteHistory);
        FREE(wideHistory);
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
        setStringCapacities("do");
    }
    return 0;
}

/* Get the next command line */
int nextCmd(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int promptSize;

    int newSizeToAlloc = 0;

    char *multiByteHistory = NULL;

    wchar_t *wideHistory = NULL;

    /* Go the beginning of the current edited line then clearn the screen from */
    while (*cursorLocation)
    {
        gotoLeft(*commandLine, cursorLocation);
    }
    setStringCapacities("up");
    setStringCapacities("do");
    setStringCapacities("cd");
    /* Get the new command line then display it */
    promptSize = printPrompt(WRITE_PROMPT);
    multiByteHistory = getNextLineInScilabHistory();
    if (multiByteHistory != NULL)
    {
        wideHistory = to_wide_string(multiByteHistory);
        if (wideHistory != NULL)
        {
            /* Allocation by a multiple of 1024 */
            newSizeToAlloc = wcslen(wideHistory) / 1024 + 1;
            FREE(*commandLine);
            *commandLine = MALLOC(sizeof(**commandLine) * (newSizeToAlloc * 1024));
            if (*commandLine != NULL)
            {
                wcscpy(*commandLine, wideHistory);
            }
        }
        FREE(multiByteHistory);
        FREE(wideHistory);
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
        setStringCapacities("do");
    }
    return 0;
}
