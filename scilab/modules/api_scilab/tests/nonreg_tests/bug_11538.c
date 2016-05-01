/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#include "api_scilab.h"

int sci_bug_11538(char* fname, void* pvApiCtx)
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

    FREE(pstData);

    if (getAllocatedSingleWideString(pvApiCtx, piAddr, &pwstData))
    {
        Scierror(999, "Failed to get data as wide string");
        return 1;
    }

    FREE(pwstData);
    return 0;
}
