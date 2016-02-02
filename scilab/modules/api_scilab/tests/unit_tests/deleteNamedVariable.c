/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

#include "api_scilab.h"

int sci_deleteNamedVariable(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet = 0;
    int* piAddr = NULL;
    char* pstVarName = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstVarName))
    {
        //error
        return 1;
    }

    if (isNamedVarExist(pvApiCtx, pstVarName))
    {
        iRet = deleteNamedVariable(pvApiCtx, pstVarName);
    }

    freeAllocatedSingleString(pstVarName);
    createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
    return 0;
}
