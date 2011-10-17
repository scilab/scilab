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
#include	"getNbrLine.h"

/* Add a character to a command line */
int addChar(t_list_cmd * cmd, int key, unsigned int *cursorLocation)
{
    unsigned int indexToMoveChar = 0;

    unsigned int promptSize;

    unsigned int sizeOfCmd = 0;

    unsigned int nbrLine = 0;

    promptSize = getPrompt(NOWRT_PRT);
    nbrLine = getNbrLine(cmd->cmd);
    sizeOfCmd = wcslen(cmd->cmd);
    if (' ' <= key)
    {
        capStr("im");
        if (sizeOfCmd && !((sizeOfCmd + 1) % 1024))
        {
            cmd->cmd = realloc(cmd->cmd, sizeof(cmd->cmd[0]) * (sizeOfCmd + 1 + 1024));
        }
        indexToMoveChar = sizeOfCmd;
        /* move each character to the next place */
        while (indexToMoveChar > *cursorLocation)
        {
            cmd->cmd[indexToMoveChar] = cmd->cmd[indexToMoveChar - 1];
            indexToMoveChar--;
        }
        /* Add the new character to the command line. */
        cmd->cmd[*cursorLocation] = (wchar_t) key;
        printf(SCI_PRINT_WCHAR, cmd->cmd[*cursorLocation]);
        sizeOfCmd++;
        cmd->cmd[sizeOfCmd] = L'\0';
        (*cursorLocation)++;
        if (!((*cursorLocation + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
        capStr("ei");
    }
    return 0;
}

/* Delete a character in the command line */
int rmChar(t_list_cmd * cmd, int key, unsigned int *cursorLocation)
{
    int indexToMoveChar;

    int promptSize;

    int sizeOfCmd = 0;

    promptSize = getPrompt(NOWRT_PRT);
    sizeOfCmd = wcslen(cmd->cmd);
/*
 * Case Backspace is pressed -> cursor must not be at the beginning of the command line
 * Case Delete is pressed -> cursor must not be at the end of line
 */
    if ((*cursorLocation && key == SCI_BACKSPACE) || ((sizeOfCmd != *cursorLocation) && key == SCI_DELETE))
    {
        if (key == SCI_BACKSPACE)
        {
            gotoLeft(cmd, cursorLocation);
        }
        indexToMoveChar = *cursorLocation;
        capStr("dc");
        while (indexToMoveChar < sizeOfCmd)
        {
            /* move each character to the previous place and print it */
            cmd->cmd[indexToMoveChar] = cmd->cmd[indexToMoveChar + 1];
            indexToMoveChar++;
        }
        cmd->cmd[indexToMoveChar] = L'\0';
    }
    return 0;
}

/* Delete all characters from cursor to the end. */
int deleteLineFromCurs(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    /* The character at the cursor is '\0' mean this is the last */
    while (cmd->cmd[*cursorLocation])
    {
        rmChar(cmd, SCI_DELETE, cursorLocation);
    }
    return 0;
}
