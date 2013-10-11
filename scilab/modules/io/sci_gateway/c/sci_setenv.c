/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "gw_io.h"
#include "api_scilab.h"
#include "setenvc.h"
#include "MALLOC.h" /* MALLOC */
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_setenv(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddr1 = NULL;
    char *pstVar = NULL;

    int *piAddr2 = NULL;
    char *pstValue = NULL;

    int ret = 0;

    Rhs = Max(0, Rhs);
    CheckRhs(2, 2);
    CheckLhs(0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr1) == 0 || isScalar(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr2) == 0 || isScalar(pvApiCtx, piAddr2) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstVar))
    {
        if (pstVar)
        {
            freeAllocatedSingleString(pstVar);
        }

        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstValue))
    {
        if (pstValue)
        {
            freeAllocatedSingleString(pstValue);
        }

        FREE(pstVar);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    ret = setenvc(pstVar, pstValue);

    FREE(pstVar);
    FREE(pstValue);

    if (createScalarBoolean(pvApiCtx, Rhs + 1, ret))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
