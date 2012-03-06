// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
#include <stdlib.h>
#include <machine.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <localization.h>
// =============================================================================
extern C2F(xerbla)(char *srname, int *info);
// =============================================================================
int sci_call_xerbla(char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    char *param1 = NULL;
    int param2 = 0;

    int retXerbla = 0;

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &param1) != 0)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        double dParam2 = 0.;
        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Integer expected.\n"), fname, 1);
            return 0;
        }
        else
        {
            if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dParam2) != 0)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            param2 = (int) dParam2;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: Integer expected.\n"), fname, 2);
        return 0;
    }

    retXerbla = C2F(xerbla)(param1, &param2);

    if (param1)
    {
        freeAllocatedSingleString(param1);
        param1 = NULL;
    }

    LhsVar(1) = 1;
    return 0;
}
// =============================================================================
