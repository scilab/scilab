/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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
/* file: sci_rubberbox.c                                                  */
/* desc : interface for rubberbox routine                                 */
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
#include "api_scilab.h"
#include "localization.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "JavaInteraction.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
#include "CurrentSubwin.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4], void* pvApiCtx);
static int getEditionMode(int rhsPos, void* pvApiCtx);
static int returnRectAndButton(const double *_piJavaValues, int _iSelectedRectSize, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4], void* pvApiCtx)
{
    SciErr sciErr;
    int rectNbRow = 0;
    int rectNbCol = 0;
    int i = 0;
    int nbDims = 0;
    double* rect = NULL;

    /* Initial rect is always in first position */
    //get variable address
    int* piAddr1 = NULL;
    double* rectStackPointer = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &rectNbRow, &rectNbCol, &rectStackPointer);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), "rubberbox", 1);
        printError(&sciErr, 0);
        return 1;
    }


    /* We have found a matrix of double within the parameters check its dims */
    nbDims = rectNbCol * rectNbRow;

    if (nbDims != 2 && nbDims != 4)
    {
        return -1;
    }


    /* pointer on the stack */
    rect = (rectStackPointer);

    /* initialize to 0 */
    for (i = 0; i < 4; i++)
    {
        initRect[i] = 0.0;
    }

    /* Set specified values */
    for (i = 0; i < nbDims; i++)
    {
        initRect[i] = rect[i];
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
static int getEditionMode(int rhsPos, void* pvApiCtx)
{
    SciErr sciErr;
    int nbRow = 0;
    int nbCol = 0;
    BOOL isEditionModeOn = FALSE;
    //get variable address
    int* piAddrrhsPos = NULL;
    int* stackPointer = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, rhsPos, &piAddrrhsPos);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of boolean at position rhsPos.
    sciErr = getMatrixOfBoolean(pvApiCtx, piAddrrhsPos, &nbRow, &nbCol, &stackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), "rubberbox", rhsPos);
        return 1;
    }


    if (nbRow * nbCol != 1)
    {
        return -1;
    }

    isEditionModeOn = *(stackPointer);
    if (isEditionModeOn)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
/*--------------------------------------------------------------------------*/
static int returnRectAndButton(const double *_piJavaValues, int _iSelectedRectSize, void* pvApiCtx)
{
    SciErr sciErr;
    int zero = 0;
    int one = 1;
    int i = 0;
    int j = 0;
    double* rectStackPointer = NULL;

    // button < 0 means user cancelled selection so return [] or [], []
    if (_piJavaValues[0] < 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, zero, zero, &rectStackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), "rubberbox");
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        if (nbOutputArgument(pvApiCtx) >= 2)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, zero, zero, &rectStackPointer);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), "rubberbox");
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
        }
        ReturnArguments(pvApiCtx);
        return 0;
    }

    /* return rectangle */
    sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, one, _iSelectedRectSize, &rectStackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), "rubberbox");
        return 1;
    }

    for (i = 0; i < _iSelectedRectSize / 2; i++)
    {
        j = i + _iSelectedRectSize / 2;
        (rectStackPointer)[i] = _piJavaValues[i + 1];
        (rectStackPointer)[j] = _piJavaValues[i + 4];
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    /* return button */
    if (nbOutputArgument(pvApiCtx) >= 2)
    {
        double* buttonStackPointer = NULL;
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, one, one, &buttonStackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), "rubberbox");
            return 1;
        }

        *(buttonStackPointer) = _piJavaValues[0];
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_rubberbox(char * fname, void *pvApiCtx)
{
    /* [final_rect, btn] = rubberbox([initial_rect],[edition_mode]) */

    double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
    int initialRectSize = 0;

    double *piJavaValues = NULL;
    int iFigureUID = 0;
    int* piFigureUID = &iFigureUID;
    int iSubwinUID = getOrCreateDefaultSubwin();
    int iView = 0;
    int* piView = &iView;

    BOOL bClickMode = TRUE;

    CheckInputArgument(pvApiCtx, 0, 2);
    CheckOutputArgument(pvApiCtx, 1, 2);
    // iView == 1 => 2D
    // else 3D
    getGraphicObjectProperty(iSubwinUID, __GO_VIEW__, jni_int, (void**)&piView);
    iFigureUID = getParentObject(iSubwinUID);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        /* rubberbox() */
        bClickMode = TRUE;
        initialRectSize = 0;
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        // Check we are running 2D view rubberbox,
        // Otherwise initial_rect and edition_mode are not usable.
        if (iView == 1) // 3D
        {
            Scierror(999, _("%s: Can not run rubberbox on a 3D view with initial_rect or edition_mode option. See help for more information.\n"), fname);
            return -1;
        }
        /* rubberbox(initial_rect) or rubberbox(edition_mode) */
        if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
        {
            /* rubberbox(initial_rect) */
            if (getInitialRectangle(initialRect, pvApiCtx) == 1)
            {
                bClickMode = TRUE;
                initialRectSize = 4;
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), fname, 1, 2, 4);
                return -1;
            }
        }
        else if (checkInputArgumentType(pvApiCtx, 1, sci_boolean))
        {
            /* rubberbox(editionMode) */
            int editionModeStatus = getEditionMode(1, pvApiCtx);
            initialRectSize = 0;
            if (editionModeStatus == 1)
            {
                /* rubberbox(%t) */
                bClickMode = FALSE;
            }
            else if (editionModeStatus == 0)
            {
                /* rubberbox(%f) */
                bClickMode = TRUE;
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected\n."), fname, 1);
                return -1;
            }
        }
        else
        {
            /* Wrong parameter specified, neither edition mode nor initial rect */
            Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector or a boolean expected.\n"), fname, 1);
            return -1;
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        /* rubberbox(initial_rect, edition_mode) */

        /* Default values, initial rect and edition mode to false */
        double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
        int editionModeStatus = 0;

        // Check we are running 2D view rubberbox,
        // Otherwise initial_rect and edition_mode are not usable.
        if (iView == 1) // 3D
        {
            Scierror(999, _("%s: Can not run rubberbox on a 3D view with initial_rect or edition_mode option. See help for more information.\n"), fname);
            return -1;
        }

        if ((!checkInputArgumentType(pvApiCtx, 1, sci_matrix)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, 1);
            return -1;
        }

        if ((!checkInputArgumentType(pvApiCtx, 2, sci_boolean)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
            return -1;
        }

        /* Getting initial rect */
        if (getInitialRectangle(initialRect, pvApiCtx) != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), fname, 1, 2, 4);
            return -1;
        }

        /* Getting edition mode */
        editionModeStatus = getEditionMode(2, pvApiCtx);

        if (editionModeStatus == 1)
        {
            /* rubberbox(initial_rect, %t) */
            /*
            ** Force click mode !!! Don't know to drag
            ** if first point is set using init rect
            */
            bClickMode = TRUE;
        }
        else if (editionModeStatus == 0)
        {
            /* rubberbox(initial_rect, %f) */
            bClickMode = TRUE;
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected\n."), fname, 2);
            return -1;
        }

    }


    if (bClickMode == TRUE)
    {
        piJavaValues = javaClickRubberBox(iFigureUID, initialRect, initialRectSize);
    }
    else
    {
        piJavaValues = javaDragRubberBox(iFigureUID);
    }
    /* Put values into the stack and return */
    if (iView == 1)
    {
        return returnRectAndButton(piJavaValues, 6, pvApiCtx);
    }
    else
    {
        return returnRectAndButton(piJavaValues, 4, pvApiCtx);
    }
}

/*--------------------------------------------------------------------------*/
