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

#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <term.h>
#include <signal.h>
#include "MALLOC.h"
#include "getKey.h"
#include "history.h"
#include "reader.h"
#include "cap_func.h"
#include "goto_func.h"
#include "charctl.h"
#include "init_tc_shell.h"
#include "aff_prompt.h"
#include "get_signal.h"
#include "charEncoding.h"

/* Create a new link for the history. */
static DoublyLinkedList *getNewHist(DoublyLinkedList * cmd)
{
    cmd->data = List_append(cmd, cmd->data);
    while (cmd->next != NULL)
    {
        cmd = cmd->next;
    }
    return cmd;
}

/*
 * Initialise current line edited
 * Return the line sent by user
 */
static DoublyLinkedList *initUsrInput(DoublyLinkedList * listCmd)
{
    wchar_t *wcsData = NULL;

    listCmd = getNewCmd(listCmd);
    getPrompt(WRT_PRT);
/* Hardcoded value */
    wcsData = MALLOC(sizeof(wchar_t) * 1024);
    *wcsData = L'\0';
    listCmd->data = wcsData;
/* please comment */
    getCmd(&listCmd);
    if (listCmd->next != NULL)
    {
        listCmd = getNewHist(listCmd);
    }
    deleteHistory(listCmd, 200);
    return listCmd;
}

/*
 * Get final command line, then convert wchar to multi-byte char
 * return a string or NULL in case there's an error.
 */
char *getCmdLine(void)
{
    static DoublyLinkedList *history = NULL;

    char *returnedCharString = NULL;

    wchar_t *dupCmdToConvert;

    /* TODO: Catch SIGINT */
    signal(SIGWINCH, getNewTerm);
    history = initUsrInput(history);
    if (history == NULL)
    {
        return NULL;
    }
    dupCmdToConvert = wcsdup(history->data);
    if (dupCmdToConvert == NULL)
    {
        fprintf(stderr, "An error deleted your command line.\n");
        return NULL;
    }
    returnedCharString = wide_string_to_UTF8(dupCmdToConvert);
    return returnedCharString;
}
