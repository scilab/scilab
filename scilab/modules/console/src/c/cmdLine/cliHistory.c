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
#include <unistd.h>
#include <term.h>
#include <termios.h>
#include <wchar.h>
#include "termcapManagement.h"
#include "gotoFunctions.h"
#include "cliPrompt.h"
#include "cliHistory.h"
#include "HistoryManager.h"
#include "sci_malloc.h"
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
