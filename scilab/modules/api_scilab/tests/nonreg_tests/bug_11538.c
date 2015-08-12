/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#include "api_scilab.h"

int sci_bug_11538(char* fname, int length)
{
    SciErr sciErr;
    int* piAddr = NULL;
    wchar_t* pwstData = NULL;
    char* pstData = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstData))
    {
        Scierror(999, "Failed to get data as ascii string");
        return 1;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddr, &pwstData))
    {
        Scierror(999, "Failed to get data as wide string");
        return 1;
    }

    LhsVar(0) = 1;
    return 0;
}
