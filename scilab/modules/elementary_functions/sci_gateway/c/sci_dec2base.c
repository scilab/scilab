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
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_dec2base(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;
    int m = 0, n = 0;

    double *dValues = NULL;
    char **convertedValues = NULL;
    unsigned int iBaseUsed = 0;
    double dBaseUsed = 0.;
    unsigned int nbDigits = 0;
    error_convertbase err = ERROR_CONVERTBASE_NOK;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 3)
    {
        double dParamThree = 0.;
        unsigned int iParamThree = 0;
        int *piAddressVarThree = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 1;
        }

        if (!isDoubleType(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 3);
            return 1;
        }

        if (!isScalar(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), fname, 3);
            return 1;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarThree, &dParamThree) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 1;
        }

        iParamThree = (unsigned int)dParamThree;

        if (dParamThree != (double)iParamThree)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 3);
            return 1;
        }

        nbDigits = iParamThree;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
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

    if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dBaseUsed) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    iBaseUsed = (unsigned int)dBaseUsed;
    if (dBaseUsed != (double)iBaseUsed)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
        return 1;
    }

    if (iBaseUsed < 2 || iBaseUsed > 36)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be between %d and %d."), fname, 2, 2, 36);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
    {
        if (createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 1;
        }
        else
        {
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
            return 0;
        }
    }

    if (!isDoubleType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), fname, 1);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m, &n , &dValues);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    convertedValues = convertMatrixOfDec2Base(dValues, m * n, iBaseUsed, nbDigits, &err);
    if ((err != ERROR_CONVERTBASE_OK) || (convertedValues == NULL))
    {
        freeArrayOfString(convertedValues, m * n);
        convertedValues = NULL;

        switch (err)
        {
            case ERROR_CONVERTBASE_NOT_INTEGER_VALUE:
                Scierror(999, _("%s: Wrong value(s) for input argument #%d: A matrix of positive integer values expected.\n"), fname, 1);
                return 1;

            case ERROR_CONVERTBASE_NOT_IN_INTERVAL:
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be between 0 and 2^52.\n"), fname, 1);
                return 1;

            case ERROR_CONVERTBASE_ALLOCATION:
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 1;

            case ERROR_CONVERTBASE_NOK:
            default:
                Scierror(999, _("%s: Wrong value for input argument #%d: cannot convert value(s).\n"), fname, 1);
                return 1;
        }
    }

    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n, (char const * const*) convertedValues);
    freeArrayOfString(convertedValues, m * n);
    convertedValues = NULL;

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
