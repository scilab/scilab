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

int read_write_boolean(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i;
    //first variable info : real matrix of double
    int iRows       = 0;
    int iCols       = 0;
    int *piAddr     = NULL;
    int* piBool     = NULL;

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
    sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &piBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Do something with data
    for (i = 0 ; i < iRows * iCols ; i++)
    {
        piBool[i] = piBool[i] == 0 ? 1 : 0;
    }

    sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, piBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
