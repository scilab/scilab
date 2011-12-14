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

#include <curses.h>
#include <stdlib.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
#include "cap_func.h"
#include "goto_func.h"
#include "aff_prompt.h"
#include "charctl.h"
#include "getNbrLine.h"

/* Add a character to a command line */
int addChar(wchar_t ** CommandLine, int key, unsigned int *cursorLocation)
{
    unsigned int indexToMoveChar = 0;

    unsigned int promptSize;

    unsigned int sizeOfCmd = 0;

    unsigned int nbrLine = 0;

    promptSize = getPrompt(NOWRT_PRT);
    nbrLine = getNbrLine(*CommandLine);
    sizeOfCmd = wcslen(*CommandLine);
    if (L' ' <= key || key == L'\n')
    {
        if (key == L'\n')
        {
            *cursorLocation = sizeOfCmd;
        }
        capStr("im");
        if (sizeOfCmd && !((sizeOfCmd + 1) % 1024)) /* TODO: Comment */
        {
            *CommandLine = realloc(*CommandLine, sizeof(wchar_t) * (sizeOfCmd + 1 + 1024));
        }
        indexToMoveChar = sizeOfCmd;
        /* move each character to the next place */
        while (indexToMoveChar > *cursorLocation)
        {
            (*CommandLine)[indexToMoveChar] = (*CommandLine)[indexToMoveChar - 1];
            indexToMoveChar--;
        }
        /* Add the new character to the command line. */
        (*CommandLine)[*cursorLocation] = (wchar_t) key;
        printf("%lc", (*CommandLine)[*cursorLocation]);
        sizeOfCmd++;
        (*CommandLine)[sizeOfCmd] = L'\0';
        (*cursorLocation)++;
        capStr("ei");
        /* To prevent a lost cursor (if cursor reach the last column of the term) */
        if ((*CommandLine)[*cursorLocation] && (*CommandLine)[*cursorLocation] != L'\n')
        {
            /* Write the next character in the string then move the cursor left */
            printf("%lc\b", (*CommandLine)[*cursorLocation]);
        }
        else
        {
            /* If there is none, write a space then move the cursor left */
            printf(" \b");
        }
    }
    return 0;
}

/* Delete a character in the command line */
int rmChar(wchar_t * CommandLine, int key, unsigned int *cursorLocation)
{
    unsigned int indexToMoveChar;

    int promptSize;

    unsigned int sizeOfCmd = 0;

    promptSize = getPrompt(NOWRT_PRT);
    sizeOfCmd = wcslen(CommandLine);
/*
 * Case Backspace is pressed -> cursor must not be at the beginning of the command line
 * Case Delete is pressed -> cursor must not be at the end of line
 */
    if ((*cursorLocation && key == SCI_BACKSPACE) || ((sizeOfCmd != *cursorLocation) && key == SCI_DELETE))
    {
        if (key == SCI_BACKSPACE)
        {
            gotoLeft(CommandLine, cursorLocation);
        }
        indexToMoveChar = *cursorLocation;
        /* Save cursor position where it must be placed */
        capStr("sc");
        while (indexToMoveChar < sizeOfCmd)
        {
            /* move each character to the previous place and print it */
            CommandLine[indexToMoveChar] = CommandLine[indexToMoveChar + 1];
            indexToMoveChar++;
        }
        CommandLine[indexToMoveChar] = L'\0';
        /* Delete sreen from cursor to the end */
        capStr("cd");
        /* write the new string */
        printf("%ls", &CommandLine[*cursorLocation]);
        /* Put cursor to the previously saved position */
        capStr("rc");
    }
    return 0;
}

/* Delete all characters from cursor to the end. */
int deleteFromCursToEndLine(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    /* set the end of the command line at the current cursor location */
    CommandLine[*cursorLocation] = '\0';
    /*
     * Clear screen from cursor to the end of the screen
     * Don't use "ce" because of multiline.
     */
    capStr("cd");
    return 0;
}

int deleteFromCursToBeginningLine(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    while (*cursorLocation)
    {
        rmChar(CommandLine, SCI_BACKSPACE, cursorLocation);
    }
    return 0;
}

int deletePreviousWordFromCurs(wchar_t * CommandLine, unsigned int *cursorLocation)
{
    /* Delete void character before cursor */
    while (*cursorLocation && (CommandLine[*cursorLocation - 1] == ' ' || CommandLine[*cursorLocation - 1] == '\t'))
    {
        rmChar(CommandLine, SCI_BACKSPACE, cursorLocation);
    }
    /* Then delete word before cursor */
    while (*cursorLocation && CommandLine[*cursorLocation - 1] != ' ' && CommandLine[*cursorLocation - 1] != '\t')
    {
        rmChar(CommandLine, SCI_BACKSPACE, cursorLocation);
    }
    return 0;
}
