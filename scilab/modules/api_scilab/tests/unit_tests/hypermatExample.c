/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int hypermatExample(char *fname, unsigned long fname_len)
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
