/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "banier.h"
#include "csignal.h"
#include "sigbas.h"
#include "scilabmode.h"
#include "getKey.h"
#include "cliPrompt.h"
#include "cliDisplayManagement.h"

/* If CTRL-C was pressed. */
void controlC_handler(int sig)
{
    int j = SIGINT;
    C2F(sigbas)(&j);
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
