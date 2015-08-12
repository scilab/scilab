/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */

#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "isnum.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "warningmode.h"

// =============================================================================
int sci_isnum(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int mOne = 0, nOne = 0;
    char **pStrs = NULL;
    BOOL *bRESULT = NULL;

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    if (strcmp(fname, "csvIsnum") == 0)
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Feature %s is obsolete.\n"), _("Warning"), fname);
            sciprint(_("%s: Please use %s instead.\n"), _("Warning"), "isnum");
            sciprint(_("%s: This feature will be permanently removed in Scilab %s\n\n"), _("Warning"), "6.0.0");
        }
    }

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
        if (pStrs)
        {
            freeAllocatedMatrixOfString(mOne, nOne, pStrs);
        }

        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    bRESULT = isNumMatrix((const char**)pStrs, mOne, nOne);

    freeAllocatedMatrixOfString(mOne, nOne, pStrs);

    if (bRESULT == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, mOne, nOne, (const int*)bRESULT);
    FREE(bRESULT);

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
