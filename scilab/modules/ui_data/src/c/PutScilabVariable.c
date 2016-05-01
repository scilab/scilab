/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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
#include <stdio.h>
#include "ui_data.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "os_string.h"


void putScilabVariable(const char* name, const char* const* lines, int rows, int cols)
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
