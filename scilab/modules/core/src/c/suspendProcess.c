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

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "scilabmode.h"
#include "cliPrompt.h"
#include "cliDisplayManagement.h"
#include "HistoryManager.h"
#include "initConsoleMode.h"
#include "suspendProcess.h"
#include "scilabmode.h"

/*
 * Function called by signal when signum is caught.
 * Reset termcaps and colors before suspending Scilab.
 */
void suspendProcess(int signum)
{
    struct sigaction signalDefaultSettings;

    struct sigaction lastSignalSettings;

    /* New structure is created to reset handler to default. */
    signalDefaultSettings.sa_handler = SIG_DFL;
    signalDefaultSettings.sa_flags = 0;
    sigemptyset(&signalDefaultSettings.sa_mask);
    /* Then apply settings. */
    sigaction(signum, &signalDefaultSettings, &lastSignalSettings);
    /* If scilab is launched in cli mode */
    if (getScilabMode() == SCILAB_NWNI || getScilabMode() == SCILAB_NW)
    {
        /* Reset termcaps and Characters display. */
        setAttr(ATTR_RESET);
        setCharDisplay(DISP_RESET);
        /* Suspend Scilab. */
        if (kill(getpid(), signum))
        {
            fprintf(stderr, "\nCannot suspend scilab\n");
        }
        else
        {
            sleep(1);
            /* Set back handler to reset settings if Scilab is suspended a new time. */
            sigaction(signum, &lastSignalSettings, NULL);
        }
    }
    else
    {
        if (kill(getpid(), signum))
        {
            fprintf(stderr, "\nCannot suspend scilab\n");
        }
    }
}

/*
 * Function called by signal when SIGCONT is caught
 * Set back Scilab colors and termcaps.
 */
void continueProcess(int signum)
{
    char *currentLine = NULL;

    /* If scilab is launched in cli mode */
    if (getScilabMode() == SCILAB_NWNI || getScilabMode() == SCILAB_NW)
    {
        /* Set termcaps and colors to Scilab Console settings. */
        setCharDisplay(DISP_LAST_SET);
        setAttr(RAW);
        /* Display Prompt. */
        printPrompt(WRITE_PROMPT);
        /* Get the current edited line and display it if there is any. */
        currentLine = getSearchedTokenInScilabHistory();
        if (currentLine != NULL)
        {
            printf("%s", currentLine);
            fflush(stdout);
        }
    }
}
