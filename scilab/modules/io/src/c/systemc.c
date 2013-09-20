/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
/* Interface with system C function */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include "spawncommand.h"
#else
#include <sys/wait.h>
#endif
#include "systemc.h"
/*--------------------------------------------------------------------------*/
int C2F(systemc)(char *command, int *stat)
{
#ifdef _MSC_VER
    *stat = CallWindowsShell(command);
#else
    int status = system(command);
    /* provide exit value of the child */
    *stat = WEXITSTATUS(status);
#endif
    return  0;
}
/*--------------------------------------------------------------------------*/
