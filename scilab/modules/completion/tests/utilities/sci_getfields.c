/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "completion.h"
#include "getPartLine.h"
/*--------------------------------------------------------------------------*/
int sci_getfields(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int rows = 0;
    int cols = 0;
    int length = 0;
    char *line = NULL;
    char **fields = NULL;
    int sizefields = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddr, &rows, &cols, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (rows != 1 || cols != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument: string expected.\n"), fname);
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddr, &rows, &cols, &length, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    line = (char*)MALLOC(sizeof(char) * (length + 1));
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &rows, &cols, &length, &line);
    if (sciErr.iErr)
    {
        FREE(line);
        printError(&sciErr, 0);
        return 0;
    }

    fields = completionOnFields(line, getPartLevel(line), &sizefields);
    FREE(line);

    if (fields != NULL)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, sizefields, 1, fields);
    }
    else
    {
        char *emptystr = "";
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &emptystr);
    }

    freeAllocatedMatrixOfString(sizefields, 1, fields);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    return 0;
}
