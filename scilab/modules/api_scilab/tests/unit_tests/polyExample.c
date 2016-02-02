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

int polyExample(char *fname, void* pvApiCtx)
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

    if (isPolyType(pvApiCtx, piAddr))
    {
        char pstVarName[64];
        int iLen = 0;

        sciErr = getPolyVariableName(pvApiCtx, piAddr, pstVarName, &iLen);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        if (isScalar(pvApiCtx, piAddr))
        {
            int iNbCoef				= 0;
            double* pdblReal	= NULL;
            double* pdblImg		= NULL;

            if (isVarComplex(pvApiCtx, piAddr))
            {
                iRet = getAllocatedSingleComplexPoly(pvApiCtx, piAddr, &iNbCoef, &pdblReal, &pdblImg);
                if (iRet)
                {
                    freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
                    return iRet;
                }

                sciErr = createComplexMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarName, 1, 1, &iNbCoef, &pdblReal, &pdblImg);
                if (sciErr.iErr)
                {
                    freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                freeAllocatedSingleComplexPoly(pdblReal, pdblImg);
            }
            else
            {
                iRet = getAllocatedSinglePoly(pvApiCtx, piAddr, &iNbCoef, &pdblReal);
                if (iRet)
                {
                    freeAllocatedSinglePoly(pdblReal);
                    return iRet;
                }

                sciErr = createMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarName, 1, 1, &iNbCoef, &pdblReal);
                if (sciErr.iErr)
                {
                    freeAllocatedSinglePoly(pdblReal);
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                freeAllocatedSinglePoly(pdblReal);
            }
        }
        else
        {
            int iRows           = 0;
            int iCols           = 0;
            int* piNbCoef       = NULL;
            double** pdblReal   = NULL;
            double** pdblImg    = NULL;

            if (isVarComplex(pvApiCtx, piAddr))
            {
                iRet = getAllocatedMatrixOfComplexPoly(pvApiCtx, piAddr, &iRows, &iCols, &piNbCoef, &pdblReal, &pdblImg);
                if (iRet)
                {
                    freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
                    return iRet;
                }

                sciErr = createComplexMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
                if (sciErr.iErr)
                {
                    freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                freeAllocatedMatrixOfComplexPoly(iRows, iCols, piNbCoef, pdblReal, pdblImg);
            }
            else
            {
                iRet = getAllocatedMatrixOfPoly(pvApiCtx, piAddr, &iRows, &iCols, &piNbCoef, &pdblReal);
                if (iRet)
                {
                    freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
                    return iRet;
                }

                sciErr = createMatrixOfPoly(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal);
                if (sciErr.iErr)
                {
                    freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
                    printError(&sciErr, 0);
                    return sciErr.iErr;
                }

                freeAllocatedMatrixOfPoly(iRows, iCols, piNbCoef, pdblReal);
            }
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    else
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
    }
    return 0;
}
