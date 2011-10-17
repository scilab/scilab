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

/* Remplacer "\a" par define */

static int caseHomeOrEndKey(t_list_cmd * listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'H':
        begLine(listCmd, cursorLocation);
        return 0;
    case L'F':
        endLine(listCmd, cursorLocation);
        return 0;
    default:
        printf("\a");
        fflush(stdout);
        return 0;
    }
}

/*
 * If second key was L'['
 * It mean this could be an arrow key or delete key.
 */

static int caseDelOrArrowKey(t_list_cmd ** listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'A':
        previousCmd(listCmd, cursorLocation);
        return 0;
    case L'B':
        nextCmd(listCmd, cursorLocation);
        return 0;
    case L'C':
        gotoRight(*listCmd, cursorLocation);
        return 0;
    case L'D':
        gotoLeft(*listCmd, cursorLocation);
        return 0;
    case L'3':
        if (getwchar() == L'~')
        {
            rmChar(*listCmd, SCI_DELETE, cursorLocation);
            return 0;
        }
    default:
        printf("\a");
        fflush(stdout);
        return 0;
    }
}

/*
 * If last key was Meta...
 */
static int caseMetaKey(t_list_cmd ** listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'f':
    case L'F':
        nextWord(*listCmd, cursorLocation);
        return 0;
    case L'b':
    case L'B':
        previousWord(*listCmd, cursorLocation);
        return 0;
    case L'[':
        return caseDelOrArrowKey(listCmd, cursorLocation);
    case L'O':
        return caseHomeOrEndKey(*listCmd, cursorLocation);
    default:
        return 0;
    }
}

/*
 * Read keyboard a first time.
 */

static int getKey(t_list_cmd ** listCmd, unsigned int *cursorLocation)
{
    int key;

    key = getwchar();
    switch (key)
    {
    case CTRL_A:
        begLine(*listCmd, cursorLocation);
        return 0;
    case CTRL_B:
        gotoLeft(*listCmd, cursorLocation);
        return 0;
    case CTRL_E:
        endLine(*listCmd, cursorLocation);
        return 0;
    case CTRL_F:
        gotoRight(*listCmd, cursorLocation);
        return 0;
    case CTRL_K:
        deleteLineFromCurs(*listCmd, cursorLocation);
        return 0;
    case CTRL_N:
        nextCmd(listCmd, cursorLocation);
        return 0;
    case CTRL_P:
        previousCmd(listCmd, cursorLocation);
        return 0;
    case ESCAPE:
        return caseMetaKey(listCmd, cursorLocation);
    case SCI_BACKSPACE:
        rmChar(*listCmd, SCI_BACKSPACE, cursorLocation);
        return 0;
    default:
        if (key == L'\n')
            return '\n';
        addChar(*listCmd, key, cursorLocation);
        return 0;
    }
}

/*
 * If there is a string the function save it.
 * else The function write the saved string.
 */
void memCmd(t_list_cmd * cmd, int cursorLocation)
{
    static t_list_cmd *memList;

    static int i;

    if (cmd != NULL)
    {
        memList = cmd;
        i = cursorLocation;
    }
    else
    {
/* TODO comment */
        i = cursorLocation;
        cursorLocation = wcslen(memList->cmd);
        printf(SCI_PRINT_WSTRING, memList->cmd);
/* TODO probably useless. We are doing a buffering word by word */
        fflush(stdout);
        while (cursorLocation != i)
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
    newCmd->bin = 0;
    return newCmd;
}

void getCmd(t_list_cmd ** listCmd)
{
    int bin;

    int cursorLocation = 0;

    bin = 1;
    while (bin)
    {
        memCmd(*listCmd, cursorLocation);
        if (getKey(listCmd, &cursorLocation) == '\n')
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
    return *cmd;
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
