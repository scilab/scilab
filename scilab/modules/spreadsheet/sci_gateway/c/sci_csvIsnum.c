/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */

#include "gw_spreadsheet.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "csvIsnum.h"
#include "MALLOC.h"

// =============================================================================
int sci_csvIsnum(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int mOne = 0, nOne = 0;
    char **pStrs = NULL;
    BOOL *bRESULT = NULL;

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, &pStrs) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    bRESULT = csv_isNumMatrix((const char**)pStrs, mOne, nOne);

    freeAllocatedMatrixOfString(mOne, nOne, pStrs);
    pStrs = NULL;

    if (bRESULT == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, mOne, nOne, (const int*)bRESULT);
    FREE(bRESULT);
    bRESULT = NULL;

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
// =============================================================================
