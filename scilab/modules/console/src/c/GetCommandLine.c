/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <io.h>
#define isatty  _isatty
#define fileno  _fileno
#else
#include <unistd.h>             /* isatty */
#endif
#include "Thread_Wrapper.h"     /* Thread should be first for Windows */
#include "BOOL.h"
#include "ConsoleRead.h"
#include "SetConsolePrompt.h"
#include "configvariable_interface.h"
#include "sci_malloc.h"
#include "prompt.h"
#include "HistoryManager.h"
#if !defined(_MSC_VER)
#include "getKey.h"
#endif
#include "initConsoleMode.h"
#include "GetCommandLine.h"
#include "TermReadAndProcess.h"
#include "os_string.h"
#include "BrowseVarManager.h"
#include "scicurdir.h"
#include "FileBrowserChDir.h"
#include "InitializeJVM.h"

/*--------------------------------------------------------------------------*/
static char Sci_Prompt[PROMPT_SIZE_MAX];
static char *tmpPrompt = NULL;
static char *__CommandLine = NULL;
/*--------------------------------------------------------------------------*/
/***********************************************************************
 * line editor
 **********************************************************************/
static void getCommandLine(void)
{
    tmpPrompt = GetTemporaryPrompt();
    GetCurrentPrompt(Sci_Prompt);

    if (__CommandLine)
    {
        FREE(__CommandLine);
        __CommandLine = NULL;
    }

    if (getScilabMode() == SCILAB_STD)
    {
        /* Send new prompt to Java Console, do not display it */
        if (tmpPrompt != NULL)
        {
            SetConsolePrompt(tmpPrompt);
        }
        else
        {
            SetConsolePrompt(Sci_Prompt);
        }
        setSearchedTokenInScilabHistory(NULL);
        /* Call Java Console to get a string */
        __CommandLine = os_strdup(ConsoleRead());
    }
    else
    {
#ifndef _MSC_VER
        if (!isatty(fileno(stdin)))
#else
        if (!isatty(fileno(stdin)) && (fileno(stdin) != -2))
#endif
        {
            __CommandLine = strdup("");
        }
        else
        {
            /* Call Term Management for NW and NWNI to get a string */
            __CommandLine = getCmdLine();
        }
    }
}

/***********************************************************************/
/*
** used by mscanf to get a line from the Scilab console
*/
char *getConsoleInputLine(void)
{
    getCommandLine();
    return os_strdup(__CommandLine);
}
