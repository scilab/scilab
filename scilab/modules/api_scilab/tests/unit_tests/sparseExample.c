/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

int sparseExample(char *fname, unsigned long fname_len)
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

    if (isSparseType(pvApiCtx, piAddr))
    {
        int iRows           = 0;
        int iCols           = 0;
        int iNbItem         = 0;
        int* piNbItemRow	= NULL;
        int* piColPos       = NULL;
        double* pdblReal	= NULL;
        double* pdblImg		= NULL;

        if (isVarComplex(pvApiCtx, piAddr))
        {
            iRet = getAllocatedComplexSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
            if (iRet)
            {
                freeAllocatedComplexSparseMatrix(piNbItemRow, piColPos, pdblReal, pdblImg);
                return iRet;
            }

            sciErr = createComplexSparseMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
            if (sciErr.iErr)
            {
                freeAllocatedComplexSparseMatrix(piNbItemRow, piColPos, pdblReal, pdblImg);
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            freeAllocatedComplexSparseMatrix(piNbItemRow, piColPos, pdblReal, pdblImg);
        }
        else
        {
            iRet = getAllocatedSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
            if (iRet)
            {
                freeAllocatedSparseMatrix(piNbItemRow, piColPos, pdblReal);
                return iRet;
            }

            sciErr = createSparseMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
            if (sciErr.iErr)
            {
                freeAllocatedSparseMatrix(piNbItemRow, piColPos, pdblReal);
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            freeAllocatedSparseMatrix(piNbItemRow, piColPos, pdblReal);
        }
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    else
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
    }
    return 0;
}
