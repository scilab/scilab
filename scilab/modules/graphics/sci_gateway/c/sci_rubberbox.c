/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_rubberbox.c                                                  */
/* desc : interface for rubberbox routine                                 */
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
#include "stack-c.h"
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
static int getInitialRectangle(double initRect[4]);
static int getEditionMode(int rhsPos);
static int returnRectAndButton(const double *_piJavaValues, int _iSelectedRectSize);
/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4])
{
    int rectNbRow = 0;
    int rectNbCol = 0;
    int rectStackPointer = 0;
    int i = 0;
    int nbDims = 0;
    double * rect = NULL;

    /* Initial rect is always in first position */
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &rectNbRow, &rectNbCol, &rectStackPointer);

    /* We have found a matrix of double within the parameters check its dims */
    nbDims = rectNbCol * rectNbRow;

    if (nbDims != 2 && nbDims != 4)
    {
        return -1;
    }


    /* pointer on the stack */
    rect = stk(rectStackPointer);

    /* intialize to 0 */
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
static int getEditionMode(int rhsPos)
{
    int stackPointer = 0;
    int nbRow = 0;
    int nbCol = 0;
    BOOL isEditionModeOn = FALSE;
    GetRhsVar(rhsPos, MATRIX_OF_BOOLEAN_DATATYPE, &nbRow, &nbCol, &stackPointer);

    if (nbRow * nbCol != 1)
    {
        return -1;
    }

    isEditionModeOn = *istk(stackPointer);
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
static int returnRectAndButton(const double *_piJavaValues, int _iSelectedRectSize)
{
    int zero = 0;
    int one = 1;
    int rectStackPointer = 0;
    int i = 0;
    int j = 0;

    // button < 0 means user cancelled selection so return [] or [], []
    if (_piJavaValues[0] < 0)
    {
        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &rectStackPointer);
        LhsVar(1) = Rhs + 1;
        if (Lhs >= 2)
        {
            CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &rectStackPointer);
            LhsVar(2) = Rhs + 2;
        }
        PutLhsVar();
        return 0;
    }

    /* return rectangle */
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &one, &_iSelectedRectSize, &rectStackPointer);
    for ( i = 0; i < _iSelectedRectSize / 2; i++)
    {
        j = i + _iSelectedRectSize / 2;
        stk(rectStackPointer)[i] = _piJavaValues[i + 1];
        stk(rectStackPointer)[j] = _piJavaValues[i + 4];
    }
    LhsVar(1) = Rhs + 1;

    /* return button */
    if (Lhs >= 2)
    {
        int buttonStackPointer = 0;
        CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &buttonStackPointer);
        *stk(buttonStackPointer) = _piJavaValues[0];
        LhsVar(2) = Rhs + 2;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_rubberbox(char * fname, unsigned long fname_len)
{

    /* [final_rect, btn] = rubberbox([initial_rect],[edition_mode]) */

    double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
    int initialRectSize = 0;

    double *piJavaValues = NULL;
    char *pFigureUID = NULL;
    char *pSubwinUID = (char*)getOrCreateDefaultSubwin();
    int iView = 0;
    int* piView = &iView;

    BOOL bClickMode = TRUE;

    CheckRhs(0, 2);
    CheckLhs(1, 2);
    // iView == 1 => 2D
    // else 3D
    getGraphicObjectProperty(pSubwinUID, __GO_VIEW__, jni_int, (void**)&piView);
    getGraphicObjectProperty(pSubwinUID, __GO_PARENT__, jni_string, (void **)&pFigureUID);

    if (Rhs == 0)
    {
        /* rubberbox() */
        bClickMode = TRUE;
        initialRectSize = 0;
    }
    else if (Rhs == 1)
    {
        // Check we are running 2D view rubberbox,
        // Otherwise initial_rect and edition_mode are not usable.
        if (iView == 1) // 3D
        {
            Scierror(999, _("%s: Can not run rubberbox on a 3D view with initial_rect or edition_mode option. See help for more information.\n"), fname);
            return -1;
        }
        /* rubberbox(initial_rect) or rubberbox(edition_mode) */
        if (GetType(1) == sci_matrix)
        {
            /* rubberbox(initial_rect) */
            if (getInitialRectangle(initialRect) == 1)
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
        else if (GetType(1) == sci_boolean)
        {
            /* rubberbox(editionMode) */
            int editionModeStatus = getEditionMode(1);
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
            /* Wrong parameter specified, neither edition mode nor intial rect */
            Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector or a boolean expected.\n"), fname, 1);
            return -1;
        }
    }
    else if (Rhs == 2)
    {
        /* rubberbox(initial_rect, edition_mode) */

        /* Default values, intial rect and edition mode to false */
        double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
        int editionModeStatus = 0;

        // Check we are running 2D view rubberbox,
        // Otherwise initial_rect and edition_mode are not usable.
        if (iView == 1) // 3D
        {
            Scierror(999, _("%s: Can not run rubberbox on a 3D view with initial_rect or edition_mode option. See help for more information.\n"), fname);
            return -1;
        }

        if (GetType(1) != sci_matrix)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, 1);
            return -1;
        }

        if (GetType(2) != sci_boolean)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
            return -1;
        }

        /* Getting initial rect */
        if (getInitialRectangle(initialRect) != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), fname, 1, 2, 4);
            return -1;
        }

        /* Getting edition mode */
        editionModeStatus = getEditionMode(2);

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
        piJavaValues = javaClickRubberBox(pFigureUID, initialRect, initialRectSize);
    }
    else
    {
        piJavaValues = javaDragRubberBox(pFigureUID);
    }
    /* Put values into the stack and return */
    if (iView == 1)
    {
        return returnRectAndButton(piJavaValues, 6);
    }
    else
    {
        return returnRectAndButton(piJavaValues, 4);
    }
}

/*--------------------------------------------------------------------------*/
