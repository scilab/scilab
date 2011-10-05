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

#include		<wchar.h>
#include		<wctype.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<unistd.h>
#include		<curses.h>
#include		<termios.h>
#include		<term.h>
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"cmd_func.h"
#include		"MALLOC.h"

static int caseHomeOrEndKey(t_list_cmd ** listCmd)
{
    switch (getwchar())
    {
    case L'H':
        return (begLine(listCmd, 0));
    case L'F':
        return (endLine(listCmd, 0));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * If second key was L'['
 * It mean this an arrow key or delete key.
 */

static int caseDelOrArrowKey(t_list_cmd ** listCmd)
{
    switch (getwchar())
    {
    case L'A':
        return (previousCmd(listCmd, 0));
    case L'B':
        return (nextCmd(listCmd, 0));
    case L'C':
        return (gotoRight(listCmd, 0));
    case L'D':
        return (gotoLeft(listCmd, 0));
    case L'3':
        if (getwchar() == L'~')
            return (rmChar(listCmd, SCI_DELETE));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * If last key was Meta...
 */
static int caseMetaKey(t_list_cmd ** listCmd)
{
    switch (getwchar())
    {
    case L'f':
    case L'F':
        return (nextWord(listCmd, 0));
    case L'b':
    case L'B':
        return (previousWord(listCmd, 0));
    case L'[':
        return (caseDelOrArrowKey(listCmd));
    case L'O':
        return (caseHomeOrEndKey(listCmd));
    default:
        printf("\a");
        fflush(stdout);
        return (0);
    }
}

/*
 * Read keyboard a first time.
 */

static int getKey(t_list_cmd ** listCmd)
{
    int key;

    key = getwchar();
    switch (key)
    {
    case CTRL_A:
        return (begLine(listCmd, key));
    case CTRL_B:
        return (gotoLeft(listCmd, key));
    case CTRL_E:
        return (endLine(listCmd, key));
    case CTRL_F:
        return (gotoRight(listCmd, key));
    case CTRL_K:
        return (deleteLineFromCurs(listCmd, key));
    case CTRL_N:
        return (nextCmd(listCmd, key));
    case CTRL_P:
        return (previousCmd(listCmd, key));
    case ESCAPE:
        return (caseMetaKey(listCmd));
    case SCI_BACKSPACE:
        return (rmChar(listCmd, SCI_BACKSPACE));
    default:
        if (key == L'\n')
            return ('\n');
        return (addChar(listCmd, key));
    }
}

/*
 * If there is a string the function save it.
 * else The function write the saved string.
 */
void memCmd(t_list_cmd ** cmd)
{
    static t_list_cmd **memList;
    static int i;

    if (cmd != NULL)
    {
        memList = cmd;
        i = (*cmd)->index;
    }
    else
    {
/* TODO comment */
        i = (*memList)->index;
        (*memList)->index = (*memList)->line;
        printf(SCI_PRINT_WSTRING, (*memList)->cmd);
/* TODO probably useless. We are doing a buffering word by word */
        fflush(stdout);
        while ((*memList)->index != i)
        {
            gotoLeft(memList, 0);
        }
    }
}

t_list_cmd *getNewCmd(t_list_cmd * lastCmd)
{
    t_list_cmd *newCmd;

    newCmd = MALLOC(sizeof(*newCmd));
    if (lastCmd != NULL)
    {
        lastCmd->next = newCmd;
        lastCmd->bin = 1;
    }
    newCmd->previous = lastCmd;
    newCmd->next = NULL;
    newCmd->line = 0;
    newCmd->bin = 0;
    newCmd->nbr_line = 0;
    return (newCmd);
}

void getCmd(t_list_cmd ** listCmd, int *key)
{
    int bin;

    (*listCmd)->index = 0;
    *key = 0;
    bin = 1;
    while (bin)
    {
        memCmd(listCmd);
        if (getKey(listCmd) == '\n')
        {
            putchar('\n');
            bin = 0;
        }
    }
}

t_list_cmd *freeCmd(t_list_cmd ** cmd)
{
    t_list_cmd *save;

    save = *cmd;
    *cmd = (*cmd)->previous;
    free(save->cmd);
    free(save);
    if (*cmd)
        (*cmd)->next = NULL;
    return (*cmd);
}

void deleteHistory(t_list_cmd * cmd, int limit)
{
    while (limit && cmd->previous)
    {
        cmd = cmd->previous;
        limit--;
    }
    if (!limit)
    {
        cmd = cmd->next;
        free(cmd->previous->cmd);
        free(cmd->previous);
        cmd->previous = NULL;
    }
}
