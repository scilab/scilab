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

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <curses.h>
#include <term.h>
#include "getKey.h"
#include "termcapManagement.h"
#include "cliPrompt.h"
#include "windowsChangeManagement.h"
#include "HistoryManager.h"

/*
 * Get new terminfo
 * sighandler_t called by signal SIGWINCH.
 */
void getNewTerm(int signum)
{
    char *termEnv;

    char *currentLine = NULL;

    setStringCapacities("cl");
    /* get new terminfo */
    if (((termEnv = getenv("TERM")) == NULL) || (tgetent(NULL, termEnv) == ERR))
    {
        fprintf(stderr, "\nCannot get terminfo databases. Termcaps are no longer available\n");
    }
    printPrompt(WRITE_PROMPT);
    currentLine = getSearchedTokenInScilabHistory();
    if (currentLine != NULL)
    {
        printf("%s", currentLine);
        fflush(stdout);
    }
    signal(signum, getNewTerm);
}
