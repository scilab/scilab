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

/* for wcwidth and wcswidth */
#define _XOPEN_SOURCE
#include <wchar.h>

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <string.h>

#include <wctype.h>
#include "BOOL.h"
#include "termcapManagement.h"
#include "cliPrompt.h"
#include "gotoFunctions.h"

/*
 * Return the cursor position of the cursor in the line.
 * Note 1: This function doesn't return the cursor position in the whole command line.
 * A line is every wide characters separated by L'\n' or L'\0'.
 * Note 2 : This function return the number of column used.
 * It may not correspond to the number of characters.
 */
static int sizeOfOneLineInTerm(wchar_t * CommandLine, unsigned int cursorLocation)
{
    unsigned int beginningOfLine = cursorLocation;

    int sizeOfLineInTerm = 0;

    wchar_t saveLastWChar = 0;

    /* Character under cursor saved */
    saveLastWChar = CommandLine[cursorLocation];
    /* Set the end of the line to the cursor position */
    CommandLine[cursorLocation] = L'\0';
    /* Looking for the beginning of the line (L'\n' or beginning of the command line) */
    while (CommandLine[beginningOfLine] != L'\n' && beginningOfLine)
    {
        beginningOfLine--;
    }
    /* If the line is the first of the command, prompt size must be added */
    if (!beginningOfLine)
    {
        sizeOfLineInTerm += printPrompt(NOWRITE_PROMPT);
    }
    /* If L'\n' is found, the line start to the next character */
    if (CommandLine[beginningOfLine] == L'\n')
    {
        beginningOfLine++;
    }
    /* Set the beginning of the line */
    CommandLine += beginningOfLine;
    /* Get the number of column used by the line */
    sizeOfLineInTerm += wcswidth(CommandLine, wcslen(CommandLine));
    /* Previously saved character put back in the command line */
    CommandLine[cursorLocation - beginningOfLine] = saveLastWChar;
    return sizeOfLineInTerm;
}

/* Move cursor to the right */
int gotoRight(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    int nbrCol;

    int sizeOfWChar = 0;

    int widthOfStringInTerm = 0;

    nbrCol = tgetnum("co");
    widthOfStringInTerm = sizeOfOneLineInTerm(CommandLine, *cursorLocation + 1);
    /* if the cursor is not at the end of the command line */
    if (*cursorLocation != wcslen(CommandLine))
    {
        /* In case the wide char occupy more than one column */
        if (CommandLine[*cursorLocation] == L'\n')
        {
            sizeOfWChar = 1;
        }
        else
        {
            sizeOfWChar = wcwidth(CommandLine[*cursorLocation]);
        }
        /* While we are not at the beginning of the character... */
        while (sizeOfWChar)
        {
            if ((widthOfStringInTerm && !(widthOfStringInTerm % nbrCol) && sizeOfWChar <= 1)    // if last column of the terminal is reached...
                    || CommandLine[*cursorLocation] == L'\n')   // ... or if the cursor will go to the next line.
            {
                /* move the cursor down. */
                setStringCapacities("do");
            }
            else
            {
                /* else, move it to the right */
                setStringCapacities("nd");
            }
            sizeOfWChar--;
        }
        (*cursorLocation)++;
    }
    /* else, if the cursor is next to the last column of the window, move it down a line */
    else if (widthOfStringInTerm && !(widthOfStringInTerm % nbrCol))
    {
        setStringCapacities("do");
    }
    return *cursorLocation;
}

/* Move cursor to the left */
int gotoLeft(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    int nbrCol;

    int sizeOfWChar = 0;

    int widthOfStringInTerm = 0;

    int i = 0;

    if (CommandLine != NULL)
    {
        i = *cursorLocation;
    }
    if (i)
    {
        /* In case the wide char occupy more than one column */
        if (CommandLine[*cursorLocation - 1] == L'\n')
        {
            /* Because L'\n' return -1 */
            sizeOfWChar = 1;
        }
        else
        {
            sizeOfWChar = wcwidth(CommandLine[*cursorLocation - 1]);
        }
        /* Get the number of move too reach the end of the line (not the end of the command) */
        if (CommandLine[*cursorLocation - 1] == L'\n')
        {
            /* Manage two consecutive L'\n' */
            if ((*cursorLocation >= 2 && CommandLine[*cursorLocation - 2] == L'\n'))
            {
                setStringCapacities("up");
                i--;
                *cursorLocation = i;
                return i;
            }
            /* If the cursor will move to a previous line separated by L'\n' */
            if (*cursorLocation > 1)
            {
                nbrCol = (sizeOfOneLineInTerm(CommandLine, *cursorLocation - 2) + 1) % tgetnum("co");
            }
            else
            {
                nbrCol = printPrompt(NOWRITE_PROMPT);
            }
        }
        else
        {
            /* If the cursor move up because of the terminal size */
            nbrCol = tgetnum("co");
        }
        widthOfStringInTerm = sizeOfOneLineInTerm(CommandLine, i);
        while (sizeOfWChar)     /* While we are not at the beginning of the character... */
        {
            if ((nbrCol && !(widthOfStringInTerm % nbrCol) && sizeOfWChar <= 1) // if last column of the terminal is reached...
                    || CommandLine[*cursorLocation - 1] == L'\n')   // ... or if the cursor will go to the previous line.
            {
                setStringCapacities("up");
                while (nbrCol)
                {
                    setStringCapacities("nd");
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
        case L'\n':
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
