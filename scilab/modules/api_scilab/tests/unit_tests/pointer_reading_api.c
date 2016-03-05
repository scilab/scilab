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
#include <stdlib.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

int read_pointer(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        //create mode
        double* pdblData    = (double*)MALLOC(sizeof(double) * 2 * 2);
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
        FREE(pvPtr);
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
