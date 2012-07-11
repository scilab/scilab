/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "ui_data.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "os_strdup.h"


void putScilabVariable(char * name, char ** lines, int rows, int cols)
{
    SciErr sciErr;

    if (rows != 0 && cols != 0)
    {
        sciErr = createNamedMatrixOfString(NULL, name, rows, cols, lines);
    }
    else
    {
        sciErr = createNamedMatrixOfDouble(NULL, name, 0, 0, NULL);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
    }
}

char * getUnnamedVariable(void)
{
    char buffer[128];
    int i = 0;

    do
    {
        sprintf(buffer, "%s%i", "unnamed", i++);
    }
    while (isNamedVarExist(NULL, buffer));

    return os_strdup(buffer);
}
