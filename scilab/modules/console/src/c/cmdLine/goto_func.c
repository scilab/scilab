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

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "BOOL.h"
#include "reader.h"
#include "cap_func.h"
#include "aff_prompt.h"
#include "goto_func.h"

/* Move cursor to the right */
int gotoRight(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    int nbrCol;

    int sizeOfWChar = 0;

    int widthOfStringInTerm = 0;

    int promptSize;

    promptSize = getPrompt(NOWRT_PRT);
    nbrCol = tgetnum("co");
    /* if the cursor is not at the end of the command line */
    if (*cursorLocation != wcslen(CommandLine))
    {
        /* In case the wide char occupy more than one column */
        sizeOfWChar = wcwidth(CommandLine[*cursorLocation]);
        widthOfStringInTerm = wcswidth(CommandLine, *cursorLocation + 1) + promptSize;
        /* If the cursor is on the last column... */
        while (sizeOfWChar)     /* While we are not at the beginning of the character... */
        {
            if (!(widthOfStringInTerm % nbrCol) && sizeOfWChar <= 1)
            {
                /* move the cursor down. */
                capStr("do");
            }
            else
            {
                /* else, move it to the right */
                capStr("nd");
            }
            sizeOfWChar--;
        }
        (*cursorLocation)++;
    }
    /* else, if the cursor is next to the last column of the window, move it down a line */
    else if (!((*cursorLocation + promptSize) % nbrCol))
    {
        capStr("do");
    }
    return *cursorLocation;
}

/* Move cursor to the left */
int gotoLeft(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    int nbrCol;

    int promptSize;

    int sizeOfWChar = 0;

    int widthOfStringInTerm = 0;

    int i = 0;

    if (CommandLine != NULL)
    {
        i = *cursorLocation;
    }
    promptSize = getPrompt(NOWRT_PRT);
    if (i)
    {
        /* In case the wide char occupy more than one column */
        sizeOfWChar = wcwidth(CommandLine[*cursorLocation - 1]);
        widthOfStringInTerm = wcswidth(CommandLine, i) + promptSize;
        while (sizeOfWChar)     /* While we are not at the beginning of the character... */
        {
            nbrCol = tgetnum("co");
            if (!(widthOfStringInTerm % nbrCol) && sizeOfWChar <= 1)
            {
                capStr("up");
                while (nbrCol)
                {
                    capStr("nd");
                    --nbrCol;
                }
            }
            else
            {
                putchar('\b');
            }
            sizeOfWChar--;
        }
        i--;
    }
    if (CommandLine != NULL)
    {
        *cursorLocation = i;
        return *cursorLocation;
    }
    return i;
}

/* Move cursor to the beginning of a line */
int begLine(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    /* While the index is not zero (meaning it's the beginning of th line) */
    while (*cursorLocation)
    {
        gotoLeft(CommandLine, cursorLocation);
    }
    return *cursorLocation;
}

/* Move cursor to the end of a line */
int endLine(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    int sizeOfCmd = 0;

    sizeOfCmd = wcslen(CommandLine);
    /* While the index is different of the size of the line */
    while (sizeOfCmd - (*cursorLocation))
    {
        gotoRight(CommandLine, cursorLocation);
    }
    return *cursorLocation;
}

static BOOL isAWideCharToJump(wchar_t wideCharToTest)
{
    /* List of characters for cursor moving word by word. */
    switch (wideCharToTest)
    {
    case L' ':
    case L'\t':
    case L'[':
    case L']':
    case L'{':
    case L'}':
    case L'(':
    case L')':
    case L'.':
    case L',':
    case L';':
        return TRUE;
    default:
        return FALSE;
    }
}

int nextWord(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    /* Passing current word... */
    while (CommandLine[*cursorLocation] && !isAWideCharToJump(CommandLine[*cursorLocation]))
    {
        gotoRight(CommandLine, cursorLocation);
    }
    /* ... then passing through characters to jump */
    while (CommandLine[*cursorLocation] && isAWideCharToJump(CommandLine[*cursorLocation]))
    {
        gotoRight(CommandLine, cursorLocation);
    }
    /* what is the point of the two declaration ? */
    return *cursorLocation;
}

int previousWord(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    /* Passing through characters to jump... */
    while (*cursorLocation && isAWideCharToJump(CommandLine[*cursorLocation - 1]))
    {
        gotoLeft(CommandLine, cursorLocation);
    }
    /* ... then going to the beginning of the word */
    while (*cursorLocation && !isAWideCharToJump(CommandLine[*cursorLocation - 1]))
    {
        gotoLeft(CommandLine, cursorLocation);
    }
    return *cursorLocation;
}
