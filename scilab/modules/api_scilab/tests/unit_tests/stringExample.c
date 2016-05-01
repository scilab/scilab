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

int stringExample(char *fname, void* pvApiCtx)
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
