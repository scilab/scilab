/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "gw_elementary_functions.h"
#include "sci_malloc.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "convertbase.h"
/*--------------------------------------------------------------------------*/

int sci_base2dec(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;
    int iValue = 0;
    double dValue = 0.;
    char **pStrs = NULL;
    int m = 0;
    int n = 0;
    double *dResults = NULL;
    int i = 0;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of string expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    if (!isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 2);
        return 1;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
        return 1;
    }

    if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    iValue = (int)dValue;
    if (dValue != (double)iValue)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
        return 1;
    }

    if (iValue < 2 || iValue > 36)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d."), fname, 2, 2, 36);
        return 1;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &m, &n, &pStrs) != 0)
    {
        if (pStrs)
        {
            freeAllocatedMatrixOfString(m, n, pStrs);
        }

        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    dResults = (double*)MALLOC(sizeof(double) * (m * n));
    if (dResults == NULL)
    {
        freeAllocatedMatrixOfString(m, n, pStrs);
        pStrs = NULL;

        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    for (i = 0; i < m * n; i++)
    {
        error_convertbase err = ERROR_CONVERTBASE_NOK;
        dResults[i] = convertBase2Dec(pStrs[i], iValue, &err);
        if (err != ERROR_CONVERTBASE_OK)
        {
            freeAllocatedMatrixOfString(m, n, pStrs);
            pStrs = NULL;

            FREE(dResults);
            dResults = NULL;

            Scierror(999, _("%s: Wrong value for input argument #%d: Valid base %d representations expected.\n"), fname, 1, iValue);
            return 1;
        }
    }

    freeAllocatedMatrixOfString(m, n, pStrs);
    pStrs = NULL;

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n, dResults);

    FREE(dResults);
    dResults = NULL;

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
