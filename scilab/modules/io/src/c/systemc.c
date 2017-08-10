/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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

/*--------------------------------------------------------------------------*/
/* Interface with system C function */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include "spawncommand.h"
#else
#include <sys/wait.h>
#include "sci_malloc.h"
#endif
#include "charEncoding.h"
#include "systemc.h"
/*--------------------------------------------------------------------------*/
int systemc(char *command, int *stat)
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
int systemcW(wchar_t* _pstCommand, int *stat)
{
#ifdef _MSC_VER
    *stat = CallWindowsShellW(_pstCommand);
#else
    char* pstTemp = wide_string_to_UTF8(_pstCommand);
    int status = system(pstTemp);
    FREE(pstTemp);
    /* provide exit value of the child */
    *stat = WEXITSTATUS(status);
#endif
    return  0;
}
/*--------------------------------------------------------------------------*/
