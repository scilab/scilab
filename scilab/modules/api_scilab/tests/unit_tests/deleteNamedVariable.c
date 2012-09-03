/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "MALLOC.h"

int sci_deleteNamedVariable(char *fname, unsigned long fname_len)
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

    createScalarBoolean(pvApiCtx, Rhs + 1, iRet);
    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
    return 0;
}
