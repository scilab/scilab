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

int stringExample(char *fname, unsigned long fname_len)
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

    if (isStringType(pvApiCtx, piAddr))
    {
        if (isScalar(pvApiCtx, piAddr))
        {
            char* pstData = NULL;

            iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
            if (iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }

            iRet = createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstData);
            if (iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }

            freeAllocatedSingleString(pstData);
        }
        else
        {
            int iRows       = 0;
            int iCols       = 0;
            char** pstData  = NULL;

            iRet = getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstData);
            if (iRet)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstData);
                return iRet;
            }

            sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pstData);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstData);
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            freeAllocatedMatrixOfString(iRows, iCols, pstData);
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    else
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
    }
    return 0;
}
