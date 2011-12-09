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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif 


void putScilabVariable(char * name, char ** lines, int rows, int cols)
{
    SciErr sciErr;

    if (rows != 0 && cols != 0)
    {
        sciErr = createNamedMatrixOfString(pvApiCtx, name, rows, cols, lines);
    }
    else
    {
        sciErr = createNamedMatrixOfDouble(pvApiCtx, name, 0, 0, NULL);
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
    while (isNamedVarExist(pvApiCtx, buffer));

    return strdup(buffer);
}
