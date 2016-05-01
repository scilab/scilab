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

int read_sparse(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i, j, k;
    int* piAddr			= NULL;
    int iRows			= 0;
    int iCols			= 0;
    int iNbItem			= 0;
    int* piNbItemRow	= NULL;
    int* piColPos		= NULL;
    double* pdblReal	= NULL;
    double* pdblImg		= NULL;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddr))
    {
        sciErr = getComplexSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
    }
    else
    {
        sciErr = getSparseMatrix(pvApiCtx, piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciprint("Sparse %d item(s)\n", iNbItem);
    k = 0;

    for (i = 0 ; i < iRows ; i++)
    {
        for (j = 0 ; j < piNbItemRow[i] ; j++)
        {
            sciprint("(%d,%d) = %f", i + 1, piColPos[k], pdblReal[k]);
            if (isVarComplex(pvApiCtx, piAddr))
            {
                sciprint(" %+fi", pdblImg[k]);
            }

            sciprint("\n");
            k++;
        }
    }

    FREE(piNbItemRow);
    FREE(piColPos);
    FREE(pdblReal);
    FREE(pdblImg);

    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = 0;
    return 0;
}
