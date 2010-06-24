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

int read_write_bsparse(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int i                   = 0;
    int j                   = 0;
    int k                   = 0;
    //first variable info : real matrix of double
    int iRows               = 0;
    int iCols               = 0;
    int *piAddr             = NULL;
    int iNbItem             = 0;
    int* piNbItemRow        = NULL;
    int* piColPos           = NULL;
    int iCol                = 0;
    int iNewCol             = 0;
    int iNewItem            = 0;
    int* piNewRow           = NULL;
    int* piNewCol           = NULL;
    //check input and output arguments
    CheckRhs(1,1);
    CheckLhs(1,1);
    //get variable address of the first input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //get size and data from Scilab memory
    sciErr = getBooleanSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //Do something with data
    //convert %T -> %F and %F -> %T
    iNewItem = (iRows * iCols) - iNbItem;
    piNewRow = (int*)MALLOC(sizeof(int) * iRows);
    piNewCol = (int*)MALLOC(sizeof(int) * iNewItem);
    for(i = 0 ; i < iRows ; i++)
    {
        piNewRow[i] = iCols - piNbItemRow[i];
        for(j = 0 ; j < iCols ; j++)
        {
            int iFind = 0;
            for(k = 0 ; k < piNbItemRow[i] ; k++)
            {
                if(piColPos[iCol + k] == (j + 1))
                {
                    iFind = 1;
                    break;
                }
            }
            if(iFind == 0)
            {
                piNewCol[iNewCol++] = (j + 1);
            }
        }
        iCol += piNbItemRow[i];
    }
    sciErr = createBooleanSparseMatrix(pvApiCtx, Rhs + 1, iRows, iCols, iNewItem, piNewRow, piNewCol);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    LhsVar(1) = Rhs + 1;
    return 0;
}
