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
#include "completeLine.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
int sci_completeline(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;
    int* piAddr6 = NULL;

    char *currentline       = NULL;
    char *stringToAdd       = NULL;
    char *filePattern       = NULL;
    char *defaultPattern    = NULL;
    int stringToAddIsPath   = FALSE;
    char *postCaretLine     = NULL;
    char *result            = NULL;

    CheckInputArgument(pvApiCtx, 5, 6);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr1, &currentline))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr2, &stringToAdd))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr3, &filePattern))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr4, &defaultPattern))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getScalarBoolean(pvApiCtx, piAddr5, &stringToAddIsPath))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A boolean expected.\n"), fname, 5);
        return 1;
    }

    if (*getNbInputArgument(pvApiCtx) == 6)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddr6, &postCaretLine))
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 6);
            return 1;
        }
    }
    else
    {
        postCaretLine = (char*)MALLOC(sizeof(char));
        postCaretLine[0] = '\0';
    }

    result = completeLine(currentline, stringToAdd, filePattern, defaultPattern, (BOOL)stringToAddIsPath, postCaretLine);

    if (result == NULL)
    {
        createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, "");
    }
    else
    {
        createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, result);
        FREE(result);
        result = NULL;
    }

    freeAllocatedSingleString(currentline);
    freeAllocatedSingleString(stringToAdd);
    freeAllocatedSingleString(filePattern);
    freeAllocatedSingleString(defaultPattern);
    if (postCaretLine)
    {
        freeAllocatedSingleString(postCaretLine);
    }

    AssignOutputVariable(pvApiCtx, 1) = *getNbInputArgument(pvApiCtx) + 1; // rhs + 1
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
