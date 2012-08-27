/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int booleanSparseExample(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr		= NULL;
    int iType		= 0;
    int iRet		= 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isBooleanSparseType(pvApiCtx, piAddr))
    {
        int iRows			= 0;
        int iCols			= 0;
        int iNbItem			= 0;
        int* piNbItemRow	= NULL;
        int* piColPos		= NULL;

        iRet = getAllocatedBooleanSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
        if (iRet)
        {
            freeAllocatedBooleanSparse(piNbItemRow, piColPos);
            return iRet;
        }

        sciErr = createBooleanSparseMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos);
        if (sciErr.iErr)
        {
            freeAllocatedBooleanSparse(piNbItemRow, piColPos);
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        freeAllocatedBooleanSparse(piNbItemRow, piColPos);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    return 0;
}
