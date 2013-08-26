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
#include <stdlib.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int read_pointer(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        //create mode
        double* pdblData    = (double*)malloc(sizeof(double) * 2 * 2);
        pdblData[0]			= 1;
        pdblData[1]			= 3;
        pdblData[2]			= 2;
        pdblData[3]			= 4;
        sciErr = createPointer(pvApiCtx, nbInputArgument(pvApiCtx) + 1, (void*)pdblData);
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        //read mode
        int iType			= 0;
        int* piAddr			= NULL;
        void* pvPtr			= NULL;
        double* pdblData	= NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getPointer(pvApiCtx, piAddr, &pvPtr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        pdblData = (double*)pvPtr;
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 2, 2, pdblData);
    }
    else
    {
        return 0;
    }
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
