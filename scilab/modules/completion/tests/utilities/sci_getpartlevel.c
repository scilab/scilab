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
#include "BOOL.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "getPartLine.h"
/*--------------------------------------------------------------------------*/
int sci_getpartlevel(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int* piAddr     = NULL;
    char* pcInput   = NULL;
    char* pcOutput  = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pcInput))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A scalar string expected.\n"), fname, 1);
        return 1;
    }

    pcOutput = getPartLevel(pcInput);
    freeAllocatedSingleString(pcInput);

    if (pcOutput == NULL)
    {
        createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, "");
    }
    else
    {
        createSingleString(pvApiCtx, *getNbInputArgument(pvApiCtx) + 1, pcOutput);
        FREE(pcOutput);
        pcOutput = NULL;
    }

    AssignOutputVariable(pvApiCtx, 1) = 2; // rhs + 1
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
