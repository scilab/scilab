/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "getcommandlineargs.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
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
                argv[i] = strdup(scilab_argv[i]);
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
