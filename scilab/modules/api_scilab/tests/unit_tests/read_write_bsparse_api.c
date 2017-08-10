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
#include "sci_malloc.h"

int read_write_bsparse(char *fname, void* pvApiCtx)
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
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //get variable address of the first input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //get size and data from Scilab memory
    sciErr = getBooleanSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Do something with data
    //convert %T -> %F and %F -> %T
    iNewItem = (iRows * iCols) - iNbItem;
    piNewRow = (int*)MALLOC(sizeof(int) * iRows);
    piNewCol = (int*)MALLOC(sizeof(int) * iNewItem);

    for (i = 0 ; i < iRows ; i++)
    {
        piNewRow[i] = iCols - piNbItemRow[i];
        for (j = 0 ; j < iCols ; j++)
        {
            int iFind = 0;
            for (k = 0 ; k < piNbItemRow[i] ; k++)
            {
                if (piColPos[iCol + k] == (j + 1))
                {
                    iFind = 1;
                    break;
                }
            }

            if (iFind == 0)
            {
                piNewCol[iNewCol++] = (j + 1);
            }
        }

        iCol += piNbItemRow[i];
    }

    FREE(piNbItemRow);
    FREE(piColPos);

    sciErr = createBooleanSparseMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, iNewItem, piNewRow, piNewCol);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    FREE(piNewRow);
    FREE(piNewCol);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
