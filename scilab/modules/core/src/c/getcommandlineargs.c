/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORET
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

#include <string.h>
#include "getcommandlineargs.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int scilab_nbargs = 0;
static char *scilab_argv[MAXCMDTOKENS];
/*--------------------------------------------------------------------------*/
char ** getCommandLineArgs(int *nbargs)
{
    char **argv = NULL;
    int i = 0;

    *nbargs = 0;
    if (scilab_nbargs > 0)
    {
        *nbargs = scilab_nbargs;

        argv = (char **)MALLOC(sizeof(char*) * scilab_nbargs);
        if (argv)
        {
            for (i = 0; i < scilab_nbargs; i++)
            {
                argv[i] = os_strdup(scilab_argv[i]);
            }
        }
    }
    return argv;
}
/*--------------------------------------------------------------------------*/
int setCommandLineArgs(char **argv, int nbargs)
{
    int i = 0;

    if (nbargs >= MAXCMDTOKENS)
    {
        nbargs = MAXCMDTOKENS;
    }

    for (i = 0; i < nbargs; i++)
    {
        scilab_argv[i] = argv[i];
    }
    scilab_nbargs = nbargs;
    return 0;
}
/*--------------------------------------------------------------------------*/
