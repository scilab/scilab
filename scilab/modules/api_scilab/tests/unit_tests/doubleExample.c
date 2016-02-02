/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

int doubleExample(char *fname, void* pvApiCtx)
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
    else if (isDoubleType(pvApiCtx, piAddr))
    {
        if (isScalar(pvApiCtx, piAddr))
        {
            double dblReal	= 0;
            double dblImg   = 0;

            if (isVarComplex(pvApiCtx, piAddr))
            {
                iRet = getScalarComplexDouble(pvApiCtx, piAddr, &dblReal, &dblImg);
                if (iRet)
                {
                    return iRet;
                }

                iRet = createScalarComplexDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dblReal, dblImg);
                if (iRet)
                {
                    return iRet;
                }
            }
            else
            {
                iRet = getScalarDouble(pvApiCtx, piAddr, &dblReal);
                if (iRet)
                {
                    return iRet;
                }

                iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dblReal);
                if (iRet)
                {
                    return iRet;
                }
            }
        }
        else
        {
            int iRows			= 0;
            int iCols			= 0;
            double* pdblReal	= NULL;
            double* pdblImg		= NULL;

            if (isVarComplex(pvApiCtx, piAddr))
            {
                sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                sciErr = createComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pdblReal, pdblImg);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
            }
            else
            {
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pdblReal);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }
            }
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }

    return 0;
}
