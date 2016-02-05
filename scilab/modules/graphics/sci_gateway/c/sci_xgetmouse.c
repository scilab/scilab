/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

/*------------------------------------------------------------------------*/
/* file: sci_xgetmouse.c                                                  */
/* desc : interface for sci_xgetmouse routine                             */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetProperty.h" /* sciGetNum */
#include "CallJxgetmouse.h"
#include "FigureList.h"
#include "axesScale.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"

#include "BuildObjects.h"
#include "CurrentSubwin.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
/*--------------------------------------------------------------------------*/
int sci_xgetmouse(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    double* l2 = NULL;

    int  m1 = 1, n1 = 3;
    int mouseButtonNumber = 0;
    int windowsID = 0;
    int sel[2], m = 0, n = 0;

    int pixelCoords[2];
    double userCoords2D[2] = {0.0, 0.0};

    int selPosition = 0;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 2);

    switch (nbInputArgument(pvApiCtx))
    {
        case 1:
            if (checkInputArgumentType(pvApiCtx, 1, sci_boolean))
            {
                selPosition = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Boolean vector expected.\n"), fname, 1);
                return FALSE;
            }
            break;
        default:
            // Call Java xgetmouse
            // No need to set any option.
            break;
    }

    // Select current figure or create it
    getOrCreateDefaultSubwin();

    // Call Java to get mouse information
    if (selPosition != 0)
    {
        int* l1Sel = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, selPosition, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of boolean at position selPosition.
        sciErr = getMatrixOfBoolean(pvApiCtx, piAddrl1, &m, &n, &l1Sel);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), fname, selPosition);
            return 1;
        }

        //CheckDims
        if (m * n != 2 || 1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, selPosition, 2, 1);
            return 1;
        }

        sel[0] = (int)l1Sel[0];
        sel[1] = (int)l1Sel[1];

        // Call Java xgetmouse
        CallJxgetmouseWithOptions(sel[0], sel[1]);
    }
    else
    {
        CallJxgetmouse();
    }

    // Get return values
    mouseButtonNumber = getJxgetmouseMouseButtonNumber();
    pixelCoords[0] = (int) getJxgetmouseXCoordinate();
    pixelCoords[1] = (int) getJxgetmouseYCoordinate();

    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &l1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    // No need to calculate coordinates if callback or close is trapped
    if (mouseButtonNumber == -1000 || mouseButtonNumber == -2)
    {
        l1[0] = -1;
        l1[1] = -1;
        l1[2] = (double) mouseButtonNumber;
    }
    else
    {
        // Convert pixel coordinates to user coordinates
        int iClickedSubwinUID = getCurrentSubWin();
        updateSubwinScale(iClickedSubwinUID);
        sciGet2dViewCoordFromPixel(iClickedSubwinUID, pixelCoords, userCoords2D);

        l1[0] = userCoords2D[0];
        l1[1] = userCoords2D[1];
        l1[2] = (double) mouseButtonNumber;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    switch (Lhs)
    {
        case 1:
            ReturnArguments(pvApiCtx);
            return 0;
        case 2:
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, m1, m1, &l2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            l2[0] = windowsID; /* this is the window number */
            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
            ReturnArguments(pvApiCtx);
            return 0;
    }
    ReturnArguments(pvApiCtx);
    return -1;
}

/*--------------------------------------------------------------------------*/
