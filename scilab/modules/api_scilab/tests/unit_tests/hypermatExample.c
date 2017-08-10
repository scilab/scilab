/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
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
#include "localization.h"

int hypermatExample(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iType   = 0;
    int iRet    = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isEmptyMatrix(pvApiCtx, piAddr))
    {
        iRet = createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1);
        if (iRet)
        {
            return iRet;
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
    }
    else if (isHypermatType(pvApiCtx, piAddr))
    {
        int * dims = NULL;
        int ndims;
        double* pdblReal = NULL;
        double* pdblImg = NULL;

        if (isHypermatComplex(pvApiCtx, piAddr))
        {
            sciErr = getComplexHypermatOfDouble(pvApiCtx, piAddr, &dims, &ndims, &pdblReal, &pdblImg);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            sciErr = createComplexHypermatOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, pdblReal, pdblImg);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }
        }
        else
        {
            sciErr = getHypermatOfDouble(pvApiCtx, piAddr, &dims, &ndims, &pdblReal);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            sciErr = createHypermatOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, pdblReal);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }

    return 0;
}
