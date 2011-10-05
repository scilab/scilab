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
int addChar(t_list_cmd ** cmd, int key)
{
    int indexToMoveChar = 0;
    int promptSize;
    int saveLine;

    promptSize = getPrompt(NOWRT_PRT);
    if (' ' <= key)
    {
        if ((*cmd)->line && !((*cmd)->line % 1024))
        {
            (*cmd)->cmd = realloc((*cmd)->cmd, (*(*cmd)->cmd) * (*cmd)->line + 1024);
        }
        indexToMoveChar = (*cmd)->line;
        /* move each character to the next place */
        while (indexToMoveChar > (*cmd)->index)
        {
            (*cmd)->cmd[indexToMoveChar] = (*cmd)->cmd[indexToMoveChar - 1];
            indexToMoveChar--;
        }
        /* Add the new character to the command line. */
        (*cmd)->cmd[(*cmd)->index] = (wchar_t) key;
        printf(SCI_PRINT_WCHAR, (*cmd)->cmd[(*cmd)->index]);
        saveLine = (*cmd)->nbr_line;
        (*cmd)->line++;
        /*
         * Nummber of line is equal to :
         * the size of the command line plus the size of the prompt
         * divided by the number of column in the window
         * Plus one.
         */
        (*cmd)->nbr_line = 1 + ((*cmd)->line + promptSize + 1) / tgetnum("co");
        (*cmd)->cmd[(*cmd)->line] = L'\0';
        (*cmd)->index++;
        indexToMoveChar++;
        capStr("sc");
        printf(SCI_PRINT_WSTRING, &(*cmd)->cmd[indexToMoveChar]);
        fflush(stdout);
        capStr("rc");
        if (!(((*cmd)->index + promptSize) % tgetnum("co")))
        {
            capStr("do");
        }
    }
    return (key);
}

/* Delete a character in the command line */
int rmChar(t_list_cmd ** cmd, int key)
{
    int indexToMoveChar;
    int promptSize;

    promptSize = getPrompt(NOWRT_PRT);
/*
 * Case Backspace is pressed -> cursor must not be at the beginning of the command line
 * Case Delete is pressed -> cursor must not be at the end of line
 */
    if (((*cmd)->index && key == SCI_BACKSPACE) || (((*cmd)->line != (*cmd)->index) && key == SCI_DELETE))
    {
        if (key == SCI_BACKSPACE)
        {
            gotoLeft(cmd, key);
        }
        indexToMoveChar = (*cmd)->index;
        capStr("sc");
        while (indexToMoveChar < (*cmd)->line)
        {
            /* move each character to the previous place and print it */
            (*cmd)->cmd[indexToMoveChar] = (*cmd)->cmd[indexToMoveChar + 1];
            printf(SCI_PRINT_WCHAR, (*cmd)->cmd[indexToMoveChar]);
            indexToMoveChar++;
        }
        fflush(stdout);
        (*cmd)->cmd[indexToMoveChar] = L'\0';
        (*cmd)->line--;
        /*
         * Number of line is equal to :
         * the size of the command line plus the size of the prompt
         * divided by the number of column in the window
         * Plus one.
         */
        (*cmd)->nbr_line = 1 + ((*cmd)->line + promptSize) / tgetnum("co");
        putchar(' ');
        capStr("rc");
    }
    key = 0;
    return (key);
}

/* Delete all characters from cursor to the end. */
int deleteLineFromCurs(t_list_cmd ** cmd, int key)
{
    /* The character at the cursor is '\0' mean this is the last */
    while ((*cmd)->cmd[(*cmd)->index])
        rmChar(cmd, SCI_DELETE);
    key = 0;
    return (key);
}
