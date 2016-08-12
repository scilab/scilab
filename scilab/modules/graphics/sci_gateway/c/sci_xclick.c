/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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
/* file: sci_xclick.c                                                     */
/* desc : interface for xclick routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "CallJxclick.h"
#include "GetProperty.h"
#include "FigureList.h"
#include "axesScale.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
#include "CurrentSubwin.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xclick(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    double* rep = NULL;
    int one = 1, three = 3;
    int istr = 0;
    //int iflag = 0;

    int mouseButtonNumber = 0;
    char * menuCallback = NULL;
    int iWindowUID = 0;
    int pixelCoords[2];
    double userCoords2D[2];

    int iFigureId = 0;
    int *piFigureId = &iFigureId;

    CheckInputArgument(pvApiCtx, -1, 1);
    CheckOutputArgument(pvApiCtx, 1, 5);

    //iflag = (nbInputArgument(pvApiCtx) >= 1) ? 1 :0;

    // Select current figure or create it
    getOrCreateDefaultSubwin();

    // Call Java xclick
    CallJxclick();

    // Get return values
    mouseButtonNumber = getJxclickMouseButtonNumber();
    pixelCoords[0] = (int) getJxclickXCoordinate();
    pixelCoords[1] = (int) getJxclickYCoordinate();
    iWindowUID = getJxclickWindowID();
    menuCallback = getJxclickMenuCallback();

    // Convert pixel coordinates to user coordinates
    // Conversion is not done if the user clicked on a menu (pixelCoords[*] == -1)
    if (pixelCoords[0] != -1 && pixelCoords[1] != -1)
    {
        int iClickedSubwinUID = getCurrentSubWin();
        sciGet2dViewCoordFromPixel(iClickedSubwinUID, pixelCoords, userCoords2D);
    }
    else
    {
        userCoords2D[0] = pixelCoords[0];
        userCoords2D[1] = pixelCoords[1];
    }

    if (nbOutputArgument(pvApiCtx) == 1)
    {
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, one, three, &rep);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }

        rep[0] = (double) mouseButtonNumber;
        rep[1] = userCoords2D[0];
        rep[2] = userCoords2D[1];
    }
    else
    {
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, one, one, &rep);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }

        rep[0] = (double) mouseButtonNumber;
    }

    if (nbOutputArgument(pvApiCtx) >= 2)
    {
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, one, one, &rep);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }

        rep[0] = userCoords2D[0];
    }

    if (nbOutputArgument(pvApiCtx) >= 3)
    {
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
        // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int rep".
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, one, one, &rep);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }

        rep[0] = userCoords2D[1];
    }

    if (nbOutputArgument(pvApiCtx) >= 4)
    {
        AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 4, one, one, &rep);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }

        getGraphicObjectProperty(iWindowUID, __GO_ID__, jni_int, (void**)&piFigureId);
        rep[0] = (double) iFigureId;
    }

    if (nbOutputArgument(pvApiCtx) >= 5)
    {
        char *strRep  = NULL;
        AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
        istr = (int)strlen(menuCallback);

        if (allocSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 5, istr * one, &strRep))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            deleteJxclickString(menuCallback);
            return 1;
        }
        strncpy(strRep, menuCallback, istr);
        freeAllocatedSingleString(strRep);
    }

    deleteJxclickString(menuCallback);

    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
