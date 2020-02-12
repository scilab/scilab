/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2020 - ESI Group - Cedric Delamarre
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#include <stdio.h>
#include "resizesignal.h"
#include "scilines.h"
#include "configvariable_interface.h"

/* Set the console width read from the term size. */
void resize_handler(int sig)
{
    // save the number of lines
    int lines = getConsoleLines();
    // set the width and lines from the actual term size
    scilinesdefault();
    // avoid the message which ask if we want to continue to print
    // when the number of lines is too small
    setConsoleLines(lines);
}

int resizesignal(void)
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_sigaction = resize_handler;
    if (sigaction(SIGWINCH, &act, NULL) != 0)
    {
        fprintf(stderr, "Could not set the signal SIGWINCH to the handler.\n");
        return -1;
    }

    return 0;
}
