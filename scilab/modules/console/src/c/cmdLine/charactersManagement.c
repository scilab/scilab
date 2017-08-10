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

#include <curses.h>
#include <stdlib.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
#include "termcapManagement.h"
#include "gotoFunctions.h"
#include "cliPrompt.h"
#include "charactersManagement.h"
#include "getNbrLine.h"

/* Add a character to a command line */
int addChar(wchar_t ** CommandLine, int key, unsigned int *cursorLocation)
{
    unsigned int indexToMoveChar = 0;

    unsigned int sizeOfCmd = 0;

    sizeOfCmd = wcslen(*CommandLine);
    if (L' ' <= key || key == L'\n')
    {
        if (key == L'\n')
        {
            *cursorLocation = sizeOfCmd;
        }
        setStringCapacities("im");
        /*
         * If the size of the string size is a multiple of 1024,
         * it means the string has reached its max.
         */
        if (sizeOfCmd && !((sizeOfCmd + 1) % 1024))
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
        setStringCapacities("ei");
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

    unsigned int sizeOfCmd = 0;

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
        setStringCapacities("sc");
        while (indexToMoveChar < sizeOfCmd)
        {
            /* move each character to the previous place and print it */
            CommandLine[indexToMoveChar] = CommandLine[indexToMoveChar + 1];
            indexToMoveChar++;
        }
        CommandLine[indexToMoveChar] = L'\0';
        /* Delete sreen from cursor to the end */
        setStringCapacities("cd");
        /* write the new string */
        printf("%ls", &CommandLine[*cursorLocation]);
        /* Put cursor to the previously saved position */
        setStringCapacities("rc");
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
    setStringCapacities("cd");
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
