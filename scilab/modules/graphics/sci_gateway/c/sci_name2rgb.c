/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_graphics.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "name2rgb.h"
#include "CurrentFigure.h"
#include "createGraphicObject.h"

int sci_name2rgb(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    char* pstColor = NULL;
    double color[3];
    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr) == FALSE || isScalar(pvApiCtx, piAddr) == FALSE)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    getAllocatedSingleString(pvApiCtx, piAddr, &pstColor);
    name2rgb(pstColor, color);
    freeAllocatedSingleString(pstColor);

    if (color[0] == -1 || color[1] == -1 || color[2] == -1)
    {
        createEmptyMatrix(pvApiCtx, 2);
    }
    else
    {
        createMatrixOfDouble(pvApiCtx, 2, 1, 3, color);
    }

    AssignOutputVariable(pvApiCtx, 1) = 2;
    ReturnArguments(pvApiCtx);
    return 0;
}