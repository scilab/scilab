/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
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
#include <string.h>
#include <wchar.h>
#include "csignal.h"
#include "configvariable_interface.h"
#include "getKey.h"
/* If CTRL-C was pressed. */
void controlC_handler(int sig)
{
    setTokenInteruptExecution(DO_NOT_SEND_COMMAND);
    setExecutionBreak();
}

int csignal(void)
{

#ifdef _MSC_VER
    if (signal(SIGINT, controlC_handler) == SIG_ERR)
    {
        fprintf(stderr, "Could not set the signal SIGINT to the handler.\n");
        return -1;
    }
#else
    struct sigaction act_controlC;

    memset(&act_controlC, 0, sizeof(act_controlC));
    act_controlC.sa_sigaction = controlC_handler;
    if (sigaction(SIGINT, &act_controlC, NULL) != 0)
    {
        fprintf(stderr, "Could not set the signal SIGINT to the handler.\n");
        return -1;
    }
#endif
    return 0;
}
