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

#include <wchar.h>
#include <string.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <term.h>
#include <string.h>
#include "cliHistory.h"
#include "termcapManagement.h"
#include "gotoFunctions.h"
#include "charactersManagement.h"
#include "initConsoleMode.h"
#include "cliPrompt.h"
#include "getKey.h"
#include "sci_malloc.h"
#include "HistoryManager.h"
#include "charEncoding.h"
#include "cliDisplayManagement.h"
#include "autoCompletionCli.h"
#include "tohome.h"
#include "localization.h"

/* Set new token in order to get string changement in history */
static void updateTokenInScilabHistory(wchar_t ** commandLine)
{
    char *multiByteString = NULL;

    multiByteString = wide_string_to_UTF8(*commandLine);
    setSearchedTokenInScilabHistory(multiByteString);
    FREE(multiByteString);
}

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
    else
    {
        /*
         * In case the condition returned false, it may be SHIFT or Alt and an arrow key
         * So, the last character is taken to avoid an unintentional print of any other characters.
         */
        getwchar();
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
 * It means this could be an arrow key, a delete key or home/end key.
 */
static void caseDelOrArrowKey(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int * cmd = *commandLine;
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
                updateTokenInScilabHistory(commandLine);
                break;
            }
        //home or end key in some consoles
        case L'H':
            begLine(cmd, cursorLocation);
            break;
        case L'F':
            endLine(cmd, cursorLocation);
            break;
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

static void setCBreak(bool cbk)
{
    struct termios t;

    tcgetattr(0, &t);
    if (cbk)
    {
        t.c_cc[VMIN] = 1;       /* Wait 1 character before leaving getwchar */
        t.c_cc[VTIME] = 0;      /* Do not wait any second before leaving getwchar */
    }
    else
    {
        t.c_cc[VMIN] = 0;
        t.c_cc[VTIME] = 0;      /* Do not wait any second before leaving getwchar */
    }
    tcsetattr(0, 0, &t);
}

static void endCopyPast(wchar_t * commandLine)
{
    int sizeOfCmd = 0;

    sizeOfCmd = wcslen(commandLine);

    if (sizeOfCmd == 0)
    {
        setTokenInteruptExecution(SEND_COMMAND);
    }
    else
    {
        if (commandLine[sizeOfCmd - 1] == L'\n')
        {
            setTokenInteruptExecution(SEND_COMMAND);
        }
        else
        {
            setTokenInteruptExecution(CONTINUE_COMMAND);
        }
    }
}

/* Reset command line if CTRL-C is pressed */
static void resetCommandLine(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    char *multiByteString = NULL;

    /* Send the preivous edited line in the history */
    multiByteString = wide_string_to_UTF8(*commandLine);
    appendLineToScilabHistory(multiByteString);
    FREE(multiByteString);
    setSearchedTokenInScilabHistory(NULL);
    FREE(*commandLine);
    /* Reset command line and cursor position */
    *cursorLocation = 0;
    *commandLine = MALLOC(1024 * sizeof(**commandLine));
    **commandLine = L'\0';
    setTokenInteruptExecution(RESET_TOKEN);
}

/*
 * Read keyboard a first time.
 */
static void getKey(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    int key;

    key = getwchar();

    // Need to clear the stdin
    if (key == WEOF && feof(stdin))
    {
        clearerr(stdin);
    }

    if (getTokenInteruptExecution() == DO_NOT_SEND_COMMAND)
    {
        resetCommandLine(commandLine, cursorLocation);
        return;
    }

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
            updateTokenInScilabHistory(commandLine);
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
            deleteFromCursToEndLine(*commandLine, cursorLocation);
            updateTokenInScilabHistory(commandLine);
            break;
        case CTRL_L:
            tohome();
            printPrompt(WRITE_PROMPT);
            printf("%ls", *commandLine);
            break;
        case CTRL_N:
            nextCmd(commandLine, cursorLocation);
            break;
        case CTRL_P:
            previousCmd(commandLine, cursorLocation);
            break;
        case CTRL_U:
            deleteFromCursToBeginningLine(*commandLine, cursorLocation);
            updateTokenInScilabHistory(commandLine);
            break;
        case CTRL_W:
            deletePreviousWordFromCurs(*commandLine, cursorLocation);
            updateTokenInScilabHistory(commandLine);
            break;
        case '\t':
            autoCompletionInConsoleMode(commandLine, cursorLocation);
            updateTokenInScilabHistory(commandLine);
            break;
        case ESCAPE:
            caseMetaKey(commandLine, cursorLocation);
            break;
        case SCI_BACKSPACE:
            rmChar(*commandLine, SCI_BACKSPACE, cursorLocation);
            updateTokenInScilabHistory(commandLine);
            break;
        case WEOF:
            setCBreak(1);
            endCopyPast(*commandLine);
            break;
        default:
            /* Different keys are not in different case when it add characters to the command line */
            if (key == L'\n')
            {
                setCBreak(0);
                setCharDisplay(DISP_FAINT);
            }
            addChar(commandLine, key, cursorLocation);

            updateTokenInScilabHistory(commandLine);
            break;
    }
}

/* main command line function */
char *getCmdLine(void)
{
    char *multiByteString = NULL;

    unsigned int cursorLocation = 0;

    static wchar_t *commandLine = NULL;

    static int nextLineLocationInWideString = 0;

    printPrompt(WRITE_PROMPT);
    setCharDisplay(DISP_BRIGHT);
    setTokenInteruptExecution(RESET_TOKEN);

    if (commandLine == NULL || commandLine[nextLineLocationInWideString] == L'\0')
    {
        commandLine = MALLOC(1024 * sizeof(*commandLine));
        *commandLine = L'\0';
        nextLineLocationInWideString = 0;
    }
    else
    {
        setTokenInteruptExecution(SEND_MULTI_COMMAND);
    }
    setSearchedTokenInScilabHistory(NULL);

    while (getTokenInteruptExecution() == CONTINUE_COMMAND)
    {
        getKey(&commandLine, &cursorLocation);
    }

    cursorLocation = nextLineLocationInWideString;
    while (commandLine[cursorLocation] != L'\n' && commandLine[cursorLocation] != L'\0')
    {
        cursorLocation++;
    }

    commandLine[cursorLocation] = L'\0';

    if (getTokenInteruptExecution() == SEND_MULTI_COMMAND)
    {
        printf("%ls\n", &commandLine[nextLineLocationInWideString]);
    }

    multiByteString = wide_string_to_UTF8(&commandLine[nextLineLocationInWideString]);

    nextLineLocationInWideString = cursorLocation + 1;

    appendLineToScilabHistory(multiByteString);

    setSearchedTokenInScilabHistory(NULL);

    setCharDisplay(DISP_RESET);

    if (multiByteString && strlen(multiByteString) > 4096)
    {
        printf(_("Command is too long (more than %d characters long): could not send it to Scilab\n"), 4096);
        FREE(multiByteString);
        return NULL;
    }

    if (commandLine[nextLineLocationInWideString] == L'\0')
    {
        FREE(commandLine);
        commandLine = NULL;
    }
    return multiByteString;
}

/* set the token for the command line */
int setTokenInteruptExecution(int token)
{
    static int savedToken = RESET_TOKEN;

    if (token != CHECK_TOKEN)
    {
        savedToken = token;
    }
    return savedToken;
}

/* get the token for the command line */
int getTokenInteruptExecution(void)
{
    return setTokenInteruptExecution(CHECK_TOKEN);
}
