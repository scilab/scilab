/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "scilabmode.h"
#include "MALLOC.h"
#include "prompt.h"
#include "HistoryManager.h"
#include "storeCommand.h"       /* for ismenu() */
#include "getKey.h"
#include "initConsoleMode.h"
#include "GetCommandLine.h"
#include "TermReadAndProcess.h"
#include "UpdateBrowseVar.h"
#include "scicurdir.h"
#include "FileBrowserChDir.h"
#include "InitializeJVM.h"
#ifdef _MSC_VER
#include "mmapWindows.h"
#include "strdup_windows.h"
#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#ifdef _MSC_VER
#define IMPORT_SIGNAL __declspec(dllimport)
#else
#define IMPORT_SIGNAL extern
#endif

/*--------------------------------------------------------------------------*/
static char Sci_Prompt[PROMPT_SIZE_MAX];

static char *tmpPrompt = NULL;

static char *__CommandLine = NULL;

/*--------------------------------------------------------------------------*/

IMPORT_SIGNAL __threadSignal LaunchScilab;

IMPORT_SIGNAL __threadSignalLock *pLaunchScilabLock;

static __threadSignal TimeToWork;

static __threadSignalLock *pReadyForLaunch = NULL;

/* exit(0) must unlock */
static void release(void)
{
    if (pReadyForLaunch)
    {
        __UnLockSignal(pReadyForLaunch);
    }
}

static BOOL WatchStoreCmdThreadAlive = FALSE;

static __threadId WatchStoreCmdThread;

static BOOL WatchGetCmdLineThreadAlive = FALSE;

static __threadId WatchGetCmdLineThread;

static BOOL initialized = FALSE;

static BOOL initialJavaHooks = FALSE;

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
        __CommandLine = strdup(ConsoleRead());
    }
    else
    {
        /* Call Term Management for NW and NWNI to get a string */
        __CommandLine = getCmdLine();

    }
}

/***********************************************************************/
/*
** used by mscanf to get a line from the Scilab console
*/
char *getConsoleInputLine(void)
{
    getCommandLine();
    return strdup(__CommandLine);
}

/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void initAll(void)
{
    /* Set console mode to raw */
#ifndef _MSC_VER
    if (getScilabMode() == SCILAB_NWNI || getScilabMode() == SCILAB_NW)
    {
        initConsoleMode(RAW);
    }
#endif
    initialized = TRUE;
    pReadyForLaunch = mmap(0, sizeof(__threadSignalLock), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    atexit(release);
    __InitSignal(&TimeToWork);
    __InitSignalLock(pReadyForLaunch);
}

/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void *watchStoreCommand(void *in)
{
    __LockSignal(pLaunchScilabLock);
    __Wait(&LaunchScilab, pLaunchScilabLock);
    __UnLockSignal(pLaunchScilabLock);

    __LockSignal(pReadyForLaunch);
    WatchStoreCmdThreadAlive = FALSE;
    __Signal(&TimeToWork);
    __UnLockSignal(pReadyForLaunch);

    return NULL;
}

/***********************************************************************/
/*
** This function is threaded and wait until
** some command has been input by user using
** the shell.
*/
static void *watchGetCommandLine(void *in)
{
    getCommandLine();

    __LockSignal(pReadyForLaunch);
    WatchGetCmdLineThreadAlive = FALSE;
    __Signal(&TimeToWork);
    __UnLockSignal(pReadyForLaunch);

    return NULL;

}

/***********************************************************************/
/*
 * Previously called zzledt... Called by Fortran...
 * Now renamed to EventLoopPrompt
 * @TODO remove unused arg buf_size, menusflag, modex & dummy1
 */
void C2F(eventloopprompt) (char *buffer, int *buf_size, int *len_line, int *eof)
{

    if (getScilabMode() == SCILAB_API)
    {
        return;
    }

    if (!initialJavaHooks && getScilabMode() != SCILAB_NWNI)
    {
        initialJavaHooks = TRUE;
        // Execute the initial hooks registered in Scilab.java
        ExecuteInitialHooks();
    }

    /* if not an interactive terminal */
#ifdef _MSC_VER
    /* if file descriptor returned is -2 stdin is not associated with an input stream */
    /* example : echo plot3d | scilex -nw -e */
    if (!isatty(fileno(stdin)) && (fileno(stdin) != -2) && getScilabMode() != SCILAB_STD)
#else
    if (!isatty(fileno(stdin)) && getScilabMode() != SCILAB_STD)
#endif
    {
        /* remove newline character if there */
        if (__CommandLine != NULL)
        {
            /* read a line into the buffer, but not too
             * big */
            *eof = (fgets(__CommandLine, *buf_size, stdin) == NULL);
            *len_line = (int)strlen(__CommandLine);
            /* remove newline character if there */
            if (__CommandLine[*len_line - 1] == '\n')
            {
                (*len_line)--;
            }
            return;
        }
    }

    if (!initialized)
    {
        initAll();
    }

    __LockSignal(pReadyForLaunch);

    if (__CommandLine)
    {
        FREE(__CommandLine);
        __CommandLine = NULL;
    }
    __CommandLine = strdup("");

    if (ismenu() == 0)
    {
        if (!WatchGetCmdLineThreadAlive)
        {
            if (WatchGetCmdLineThread)
            {
                __WaitThreadDie(WatchGetCmdLineThread);
            }
            if (getScilabMode() != SCILAB_NWNI)
            {

                char *cwd = NULL;

                int err = 0;

                UpdateBrowseVar(TRUE);
                cwd = scigetcwd(&err);
                if (cwd)
                {
                    FileBrowserChDir(cwd);
                    FREE(cwd);
                }
            }
            __CreateThread(&WatchGetCmdLineThread, &watchGetCommandLine);
            WatchGetCmdLineThreadAlive = TRUE;
        }
        if (!WatchStoreCmdThreadAlive)
        {
            if (WatchStoreCmdThread)
            {
                __WaitThreadDie(WatchStoreCmdThread);
            }
            __CreateThread(&WatchStoreCmdThread, &watchStoreCommand);
            WatchStoreCmdThreadAlive = TRUE;
        }

        __Wait(&TimeToWork, pReadyForLaunch);
    }
    __UnLockSignal(pReadyForLaunch);

    /*
    ** WARNING : Old crappy f.... code
    ** do not change reference to buffer
    ** or fortran will be lost !!!!
    */
    if (__CommandLine)
    {
        strcpy(buffer, __CommandLine);
    }
    else
    {
        strcpy(buffer, "");
    }
    *len_line = (int)strlen(buffer);

    *eof = FALSE;
}
