/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
#include "sci_malloc.h"
#include "gw_graphics.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "name2rgb.h"
#include "CurrentFigure.h"
#include "createGraphicObject.h"

int sci_name2rgb(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    char* pstColor = NULL;
    double color[3];
    int i;
    int nRows;
    int nCols;
    int nRowsCols;
    char** pstColorMatrix = NULL;
    double* pColor = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr) == FALSE)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    if (isScalar(pvApiCtx, piAddr) == TRUE)
    {
        if (getAllocatedSingleString(pvApiCtx, piAddr, &pstColor))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), fname, 1);
            return 1;
        }
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
    }
    else
    {
        if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &nRows, &nCols, &pstColorMatrix))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of string expected.\n"), fname, 1);
            return 1;
        }
        nRowsCols = nRows * nCols;
        pColor = (double*)MALLOC(3 * nRowsCols * sizeof(double));
        for (i = 0; i < nRowsCols; ++i)
        {
            name2rgb(pstColorMatrix[i], color);
            if (color[0] == -1 || color[1] == -1 || color[2] == -1)
            {
                break;
            }
            pColor[i] = color[0];
            pColor[i + nRowsCols] = color[1];
            pColor[i + 2 * nRowsCols] = color[2];
        }
        freeAllocatedMatrixOfString(nRows, nCols, pstColorMatrix);
        if (color[0] == -1 || color[1] == -1 || color[2] == -1)
        {
            createEmptyMatrix(pvApiCtx, 2);
            FREE(pColor);
        }
        else
        {
            createMatrixOfDouble(pvApiCtx, 2, nRowsCols, 3, pColor);
            FREE(pColor);
        }
    }
    AssignOutputVariable(pvApiCtx, 1) = 2;
    ReturnArguments(pvApiCtx);
    return 0;
}
