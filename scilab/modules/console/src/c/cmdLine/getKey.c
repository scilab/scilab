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
#include "history.h"
#include "reader.h"
#include "cap_func.h"
#include "goto_func.h"
#include "charctl.h"
#include "init_tc_shell.h"
#include "aff_prompt.h"
#include "getKey.h"
#include "MALLOC.h"
#include "HistoryManager.h"
#include "charEncoding.h"
#include "aff_prompt.h"

void autoCompletionInConsoleMode(wchar_t ** commandLine, int *cursorLocation);

/*
 * If last key was '1'
 * it means this could be arrow key plus control key
 */
static void caseCtrlAndArrowKey(wchar_t * commandLine, unsigned int *cursorLocation)
{
    if (getwchar() == L';' && getwchar() == L'5')
    {
        switch (getwchar())
        {
        case L'C':
            nextWord(commandLine, cursorLocation);
            break;
        case L'D':
            previousWord(commandLine, cursorLocation);
            break;
        }
    }
}

static void caseHomeOrEndKey(wchar_t * commandLine, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'H':
        begLine(commandLine, cursorLocation);
        break;
    case L'F':
        endLine(commandLine, cursorLocation);
        break;
    }
}

/*
 * If second key was L'['
 * It means this could be an arrow key or delete key.
 */
static void caseDelOrArrowKey(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'A':
        previousCmd(commandLine, cursorLocation);
        break;
    case L'B':
        nextCmd(commandLine, cursorLocation);
        break;
    case L'C':
        gotoRight(*commandLine, cursorLocation);
        break;
    case L'D':
        gotoLeft(*commandLine, cursorLocation);
        break;
    case L'1':
        caseCtrlAndArrowKey(*commandLine, cursorLocation);
        break;
    case L'3':
        if (getwchar() == L'~')
        {
            rmChar(*commandLine, SCI_DELETE, cursorLocation);
            break;
        }
    }
}

/*
 * If last key was Meta...
 */
static void caseMetaKey(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'f':
    case L'F':
        nextWord(*commandLine, cursorLocation);
        break;
    case L'b':
    case L'B':
        previousWord(*commandLine, cursorLocation);
        break;
    case L'[':
        caseDelOrArrowKey(commandLine, cursorLocation);
        break;
    case L'O':
        caseHomeOrEndKey(*commandLine, cursorLocation);
        break;
    }
}

/*
 * Read keyboard a first time.
 */
int getKey(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int key;

    char *charString = NULL;

    int cursorMax = 0;

    key = getwchar();
    switch (key)
    {
    case CTRL_A:
        begLine(*commandLine, cursorLocation);
        break;
    case CTRL_B:
        gotoLeft(*commandLine, cursorLocation);
        break;
    case CTRL_D:
        rmChar(*commandLine, SCI_DELETE, cursorLocation);
        break;
    case CTRL_E:
        endLine(*commandLine, cursorLocation);
        break;
    case CTRL_F:
        gotoRight(*commandLine, cursorLocation);
        break;
    case CTRL_H:
        rmChar(*commandLine, SCI_BACKSPACE, cursorLocation);
        break;
    case CTRL_K:
        deleteLineFromCurs(*commandLine, cursorLocation);
        break;
    case CTRL_N:
        nextCmd(commandLine, cursorLocation);
        break;
    case CTRL_P:
        previousCmd(commandLine, cursorLocation);
        break;
    case '\t':
        autoCompletionInConsoleMode(commandLine, cursorLocation);
        break;
    case ESCAPE:
        caseMetaKey(commandLine, cursorLocation);
        break;
    case SCI_BACKSPACE:
        rmChar(*commandLine, SCI_BACKSPACE, cursorLocation);
        break;
    default:
        if (key == L'\n')
        {
            return 1;
        }
        addChar(*commandLine, key, cursorLocation);
        break;
    }
    return 0;
}

/*
 * If there is a string the function save it.
 * else The function write the saved string.
 */
void memCmd(wchar_t * cmd, int cursorLocation)
{
    static wchar_t *memList;

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
        cursorLocation = wcslen(memList);
        printf("%ls", memList);
        while (cursorLocation != i)
        {
            gotoLeft(memList, &cursorLocation);
        }
    }
}

char *getCmdLine(void)
{
    int bin = 1;

    char *multiByteString = NULL;

    unsigned int cursorLocation = 0;

    wchar_t *wideString = NULL;

    wideString = MALLOC(1024 * sizeof(*wideString));
    *wideString = L'\0';
    getPrompt(WRT_PRT);
    setSearchedTokenInScilabHistory(NULL);
    while (bin)
    {
        memCmd(wideString, cursorLocation);
        if (getKey(&wideString, &cursorLocation))
        {
            putchar('\n');
            bin = 0;
        }
    }
    multiByteString = wide_string_to_UTF8(wideString);
    appendLineToScilabHistory(multiByteString);
    setSearchedTokenInScilabHistory(NULL);
    return multiByteString;
}
