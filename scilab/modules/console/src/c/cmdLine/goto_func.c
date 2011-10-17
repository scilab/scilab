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

#include	<termios.h>
#include	<curses.h>
#include	<term.h>
#include	<string.h>
#include	<wchar.h>
#include	<wctype.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"aff_prompt.h"
#include	"goto_func.h"

/* Move cursor to the right */
int gotoRight(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    int nbrCol;

    int promptSize;

    promptSize = getPrompt(NOWRT_PRT);
    nbrCol = tgetnum("co");
    /* if the cursor is not at the end of the command line */
    if (*cursorLocation != wcslen(cmd->cmd))
    {
        /* If the cursor is on the last column... */
        if (!((*cursorLocation + promptSize + 1) % nbrCol))
        {
            /* move the cursor down. */
            capStr("do");
        }
        else
        {
            /* else, move it to the right */
            capStr("nd");
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

/* go to the last column of the current line */
static void findEndLine(int i, int promptSize, int nbrCol)
{
    if (!((i + promptSize) % nbrCol))
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
        capStr("le");
    }
}

/* Move cursor to the left */
int gotoLeft(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    int nbrCol;

    int promptSize;

    int i = 0;

    if (cmd != NULL)
    {
        i = *cursorLocation;
    }
    promptSize = getPrompt(NOWRT_PRT);
    nbrCol = tgetnum("co");
    if (i)
    {
        findEndLine(i, promptSize, nbrCol);
        i--;
    }
    if (cmd != NULL)
    {
        *cursorLocation = i;
        return *cursorLocation;
    }
    return i;
}

/* Move cursor to the beginning of a line */
int begLine(t_list_cmd * cmd, unsigned int *cursorLocation)
{
/* While the index is not zero (meaning it's the beginning of th line) */
    while (*cursorLocation)
    {
        gotoLeft(cmd, cursorLocation);
    }
    return *cursorLocation;
}

/* Move cursor to the end of a line */
int endLine(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    int sizeOfCmd = 0;

    sizeOfCmd = wcslen(cmd->cmd);
/* While the index is different of the size of the line */
    while (sizeOfCmd - (*cursorLocation))
    {
        gotoRight(cmd, cursorLocation);
    }
    return *cursorLocation;
}

int nextWord(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    /* Factorize cmd->cmd[*cursorLocation] */
    /* Passing current word... */
    while (cmd->cmd[*cursorLocation] && cmd->cmd[*cursorLocation] != L' ')
    {
        gotoRight(cmd, cursorLocation);
    }
    /* ... then passing through spaces */
    while (cmd->cmd[*cursorLocation] && cmd->cmd[*cursorLocation] == L' ')
    {
        gotoRight(cmd, cursorLocation);
    }
    /* what is the point of the two declaration ? */
    return *cursorLocation;
}

int previousWord(t_list_cmd * cmd, unsigned int *cursorLocation)
{
    /* Passing through spaces... */
    while (*cursorLocation && cmd->cmd[*cursorLocation - 1] == L' ')
    {
        gotoLeft(cmd, cursorLocation);
    }
    /* ... then going to the beginning of the word */
    while (*cursorLocation && cmd->cmd[*cursorLocation - 1] != L' ')
    {
        gotoLeft(cmd, cursorLocation);
    }
    /* what is the point of the two declaration ? */
    return *cursorLocation;
}
