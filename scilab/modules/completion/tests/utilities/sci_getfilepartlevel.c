/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "getPartLine.h"
/*--------------------------------------------------------------------------*/
int sci_getfilepartlevel(char *fname, void *pvApiCtx)
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

    pcOutput = getFilePartLevel(pcInput);
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
    returnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
