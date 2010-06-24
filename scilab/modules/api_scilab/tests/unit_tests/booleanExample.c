/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "MALLOC.h"

int booleanExample(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iType   = 0;
    int iRet    = 0;
    CheckRhs(1,1);
    CheckLhs(0,1);
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if(isBooleanType(pvApiCtx, piAddr))
    {
        if(isScalar(pvApiCtx, piAddr))
        {
            int iBool = 0;
            iRet = getScalarBoolean(pvApiCtx, piAddr, &iBool);
            if(iRet)
            {
                return 0;
            }
            iRet = createScalarBoolean(pvApiCtx, Rhs + 1, iBool);
            if(iRet)
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
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, iRows, iCols, piBool);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }
    return 0;
}
