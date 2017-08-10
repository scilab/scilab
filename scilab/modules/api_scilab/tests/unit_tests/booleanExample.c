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

int booleanExample(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iType   = 0;
    int iRet    = 0;

    //CheckInputArgument(pvApiCtx, 1, 1);
    //CheckOutputArgument(pvApiCtx, 0, 1);
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if (isBooleanType(pvApiCtx, piAddr))
    {
        if (isScalar(pvApiCtx, piAddr))
        {
            int iBool = 0;
            iRet = getScalarBoolean(pvApiCtx, piAddr, &iBool);
            if (iRet)
            {
                return 0;
            }
            iRet = createScalarBoolean(pvApiCtx, Rhs + 1, iBool);
            //iRet = createScalarBoolean(pvApiCtx, nbInputArgument + 1, iBool);
            if (iRet)
            {
                return 0;
            }
        }
        else
        {
            int iRows	= 0;
            int iCols	= 0;
            int *piBool	= NULL;

            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &piBool);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, iRows, iCols, piBool);
            //sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument + 1, iRows, iCols, piBool);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        LhsVar(1) = Rhs + 1;
        //AssignOutputVariable(1) = nbInputArgument + 1;
    }
    else
    {
        LhsVar(1) = 0;
        //AssignOutputVariable(1) = 0;
    }
    return 0;
}
