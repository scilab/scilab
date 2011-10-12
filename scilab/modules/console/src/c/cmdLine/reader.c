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
#include		<signal.h>
#include                "MALLOC.h"
#include		"cmd_func.h"
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"get_signal.h"
#include		"charEncoding.h"

/* comment */
wchar_t *cmdDup(t_list_cmd * cmd, wchar_t * wcs)
{
    wchar_t *dupCmd;

    int sizeOfCmd;

    /* TODO: Document */
    sizeOfCmd = wcslen(cmd->cmd);
    dupCmd = MALLOC(sizeof(*dupCmd) * (sizeOfCmd + 1));
    dupCmd[sizeOfCmd] = L'\0';
    if (wcs != NULL)
    {
        wcscpy(dupCmd, wcs);
    }
    else
    {
        wcscpy(dupCmd, cmd->cmd);
    }
    return (dupCmd);
}

/* Create a new link for the history. */
t_list_cmd *getNewHist(t_list_cmd * cmd)
{
    t_list_cmd *lastCmd;

    lastCmd = cmd;
    while (lastCmd->bin)
    {
        lastCmd = lastCmd->next;
    }
    lastCmd->cmd = cmdDup(cmd, NULL);
    return (lastCmd);
}

/*
 * Take all spaces ans tabulations at the beginning of the command line out.
 * return the cleaned command line.
 */
void *cleanVoidCharInCmd(t_list_cmd * cmd)
{
    int i;

    wchar_t *dupCmd;

    i = 0;
    /* Passing all void character at the beginning of the command line... */
    while (cmd->cmd[i] && (cmd->cmd[i] == L' ' || cmd->cmd[i] == L'\t'))
    {
        i++;
    }
    /* ... then create the new command line. */
    dupCmd = cmdDup(cmd, &cmd->cmd[i]);
    free(cmd->cmd);
    cmd->cmd = dupCmd;
    return (dupCmd);
}

/*
 * Initialise current line edited
 * Return the line sent by user
 */
t_list_cmd *initUsrInput(t_list_cmd * listCmd)
{
    int ret;

    listCmd = getNewCmd(listCmd);
    getPrompt(WRT_PRT);
/* Hardcoded value */
    listCmd->cmd = MALLOC(sizeof(*listCmd->cmd) * 1024);
    listCmd->cmd[0] = L'\0';
    ret = 0;
/* please comment */
    getCmd(&listCmd);
    cleanVoidCharInCmd(listCmd);
    if (listCmd->bin)
    {
        listCmd = getNewHist(listCmd);
    }
    deleteHistory(listCmd, 200);
    return (listCmd);
}

/*
 * Get final command line, then convert wchar to multi-byte char
 * return a string or NULL in case there's an error.
 */
char *getCmdLine(t_list_cmd ** history)
{
    char *dest;

    /* TODO: Catch SIGINT */
    signal(SIGWINCH, getNewTerm);
    *history = initUsrInput(*history);
    if (*history == NULL)
    {
        return (NULL);
    }
    dest = wide_string_to_UTF8((*history)->cmd);
    return (dest);
}
