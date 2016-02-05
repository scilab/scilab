/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
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
/* file: sci_StringBox.c                                                  */
/* desc : interface for StringBox routine                                 */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "gw_graphics.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "StringBox.h"
#include "localization.h"
#include "axesScale.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"
#include "BuildObjects.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#define DEFAULT_ANGLE 0.0

/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double* res, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double* res, void* pvApiCtx)
{
    SciErr sciErr;
    int m = 0;
    int n = 0;
    int* piAddrstackPointer = NULL;
    double* stackPointer = NULL;
    if ((!checkInputArgumentType(pvApiCtx, paramIndex, sci_matrix)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
        return -1;
    }

    /* get the handle */
    sciErr = getVarAddressFromPosition(pvApiCtx,  paramIndex, &piAddrstackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    // Retrieve a matrix of double at position  paramIndex.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrstackPointer, &m, &n, &stackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), funcName,  paramIndex);
        return -1;
    }


    if (m * n != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
        return -1;
    }

    *res = *stackPointer;
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_stringbox(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    int* piAddrstackPointer = NULL;
    long long* stackPointer = NULL;
    char** strStackPointer   = NULL;
    double* pdblStackPointer = NULL;

    int type = -1;
    int *piType = &type;

    int iParentAxes = 0;
    int* piParentAxes = &iParentAxes;
    double* textCorners = NULL;
    int two   = 2;
    int four  = 4;
    double corners[4][2]; /* the four edges of the boundingRect */

    /* The function should be called with stringbox(handle) */
    CheckInputArgument(pvApiCtx,  1, 6);
    CheckOutputArgument(pvApiCtx,  0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        int m;
        int n;
        /* A text handle should be specified */

        int iTextUID = 0;
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
            return 0;
        }

        /* get the handle */
        sciErr = getVarAddressFromPosition(pvApiCtx,  1, &piAddrstackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position  1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &m, &n, &stackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"), fname,  1, "Text");
            return 1;
        }

        if (m * n != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"), fname, 1, "Text");
            return 0;
        }

        /* Get the handle and check that this is a text handle */
        iTextUID = getObjectFromHandle((long int) * stackPointer);

        if (iTextUID == 0)
        {
            Scierror(999, _("%s: The handle is not valid.\n"), fname);
            return 0;
        }

        getGraphicObjectProperty(iTextUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (type != __GO_LABEL__ && type != __GO_TEXT__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
            return 0;
        }

        getGraphicObjectProperty(iTextUID, __GO_PARENT_AXES__, jni_int, (void **)&piParentAxes);

        updateTextBounds(iTextUID);

        /*
         * To do: performs a projection/unprojection to obtain the bounding box in object coordinates
         * but using a rotation matrix corresponding to the default rotation angles (view == 2d)
         */

        getGraphicObjectProperty(iTextUID, __GO_CORNERS__, jni_double_vector, (void **)&textCorners);

        corners[1][0] = textCorners[0];
        corners[1][1] = textCorners[1];

        corners[0][0] = textCorners[3];
        corners[0][1] = textCorners[4];

        corners[3][0] = textCorners[6];
        corners[3][1] = textCorners[7];

        corners[2][0] = textCorners[9];
        corners[2][1] = textCorners[10];
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d to %d expected.\n"), fname, 1, 3, 6);
        return 0;
    }
    else
    {
        int iParentSubwinUID = getOrCreateDefaultSubwin();
        char ** text = NULL;
        int textNbRow;
        int textNbCol;
        double xPos;
        double yPos;
        double angle = DEFAULT_ANGLE;
        int fontId;
        int *pfontId = &fontId;
        double fontSize;
        double *pfontSize = &fontSize;

        getGraphicObjectProperty(iParentSubwinUID, __GO_FONT_STYLE__, jni_int, (void**)&pfontId);
        getGraphicObjectProperty(iParentSubwinUID, __GO_FONT_SIZE__, jni_double, (void **)&pfontSize);

        /* Check that first argument is a string */
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: 2D array of strings expected.\n"), fname, 1);
            return 0;
        }
        sciErr = getVarAddressFromPosition(pvApiCtx,  1, &piAddrstackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position  1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrstackPointer, &textNbRow, &textNbCol, &strStackPointer))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname,  1);
            return 1;
        }

        /* retrieve it */
        text = strStackPointer;

        /* Second and third arguments should be scalars */
        if (getScalarFromStack(2, fname, &xPos, pvApiCtx) < 0)
        {
            freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
            return 0;
        }

        if (getScalarFromStack(3, fname, &yPos, pvApiCtx) < 0)
        {
            freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
            return 0;
        }

        if (nbInputArgument(pvApiCtx) >= 4)
        {
            /* angle is defined */
            if (getScalarFromStack(4, fname, &angle, pvApiCtx) < 0)
            {
                freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
                return 0;
            }
        }

        if (nbInputArgument(pvApiCtx) >= 5)
        {
            double fontIdD;
            /* font style is defined */
            if (getScalarFromStack(5, fname, &fontIdD, pvApiCtx) < 0)
            {
                freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
                return 0;
            }
            fontId = (int) fontIdD;
        }

        if (nbInputArgument(pvApiCtx) >= 6)
        {
            /* font size is defined */
            if (getScalarFromStack(6, fname, &fontSize, pvApiCtx) < 0)
            {
                freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
                return 0;
            }
        }

        /* compute the box */
        getTextBoundingBox(text, textNbRow, textNbCol, xPos, yPos, angle, fontId, fontSize, corners);
        freeAllocatedMatrixOfString(textNbRow, textNbCol, strStackPointer);
    }


    /* copy everything into the lhs */
    sciErr = allocMatrixOfDouble(pvApiCtx,  nbInputArgument(pvApiCtx) + 1, two, four, &pdblStackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    pdblStackPointer[0] = corners[1][0];
    pdblStackPointer[1] = corners[1][1];
    pdblStackPointer[2] = corners[0][0];
    pdblStackPointer[3] = corners[0][1];
    pdblStackPointer[4] = corners[3][0];
    pdblStackPointer[5] = corners[3][1];
    pdblStackPointer[6] = corners[2][0];
    pdblStackPointer[7] = corners[2][1];

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
#undef DEFAULT_ANGLE
