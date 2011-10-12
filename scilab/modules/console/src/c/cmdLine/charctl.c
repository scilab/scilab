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

#include	<curses.h>
#include	<stdlib.h>
#include	<term.h>
#include	<termios.h>
#include	<unistd.h>
#include	<wchar.h>
#include	<wctype.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"
#include	"charctl.h"

/* Add a character to a command line */
int addChar(t_list_cmd ** cmd, int cursorLocation)
{
    int indexToMoveChar = 0;

    int promptSize;

    int saveLine;

    int sizeOfCmd = 0;

    int nbrLine = 0;

    promptSize = getPrompt(NOWRT_PRT);
    nbrLine = getNbrLine((*cmd)->cmd);
    sizeOfCmd = wcslen((*cmd)->cmd);
    if (' ' <= cursorLocation)
    {
        capStr("im");
        if (sizeOfCmd && !((sizeOfCmd + 1) % 1024))
        {
            (*cmd)->cmd = realloc((*cmd)->cmd, sizeof((*cmd)->cmd[0]) * (sizeOfCmd + 1 + 1024));
        }
        indexToMoveChar = sizeOfCmd;
        /* move each character to the next place */
        while (indexToMoveChar > (*cmd)->index)
        {
            (*cmd)->cmd[indexToMoveChar] = (*cmd)->cmd[indexToMoveChar - 1];
            indexToMoveChar--;
        }
        /* Add the new character to the command line. */
        (*cmd)->cmd[(*cmd)->index] = (wchar_t) cursorLocation;
        printf(SCI_PRINT_WCHAR, (*cmd)->cmd[(*cmd)->index]);
        sizeOfCmd++;
        (*cmd)->cmd[sizeOfCmd] = L'\0';
        (*cmd)->index++;
        if (!(((*cmd)->index + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
        capStr("ei");
    }
    return (cursorLocation);
}

/* Delete a character in the command line */
int rmChar(t_list_cmd ** cmd, int cursorLocation)
{
    int indexToMoveChar;

    int promptSize;

    int sizeOfCmd = 0;

    promptSize = getPrompt(NOWRT_PRT);
    sizeOfCmd = wcslen((*cmd)->cmd);
/*
 * Case Backspace is pressed -> cursor must not be at the beginning of the command line
 * Case Delete is pressed -> cursor must not be at the end of line
 */
    if (((*cmd)->index && cursorLocation == SCI_BACKSPACE) || ((sizeOfCmd != (*cmd)->index) && cursorLocation == SCI_DELETE))
    {
        if (cursorLocation == SCI_BACKSPACE)
        {
            gotoLeft(cmd, cursorLocation);
        }
        indexToMoveChar = (*cmd)->index;
        capStr("dc");
        while (indexToMoveChar < sizeOfCmd)
        {
            /* move each character to the previous place and print it */
            (*cmd)->cmd[indexToMoveChar] = (*cmd)->cmd[indexToMoveChar + 1];
            indexToMoveChar++;
        }
        (*cmd)->cmd[indexToMoveChar] = L'\0';
    }
    cursorLocation = 0;
    return (cursorLocation);
}

/* Delete all characters from cursor to the end. */
int deleteLineFromCurs(t_list_cmd ** cmd, int cursorLocation)
{
    /* The character at the cursor is '\0' mean this is the last */
    while ((*cmd)->cmd[(*cmd)->index])
        rmChar(cmd, SCI_DELETE);
    cursorLocation = 0;
    return (cursorLocation);
}
