/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "completion.h"
#include "getPartLine.h"
/*--------------------------------------------------------------------------*/
int sci_getfields(char *fname, unsigned long fname_len)
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
        Scierror(999, _("%s: Wrong size for input argument: A string expected.\n"), fname);
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

    freeArrayOfString(fields, sizefields);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    return 0;
}
