// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <localization.h>

// =============================================================================
int sci_bug_11093(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    char* pstName = NULL;
    int iRet = 0;

    checkInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }


    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstName))
    {
        return 1;
    }

    iRet = isNamedVarExist(pvApiCtx, pstName);
    FREE(pstName);

    createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
    LhsVar(1) = Rhs + 1;
    return 0;
}
// =============================================================================
