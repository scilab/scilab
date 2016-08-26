/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
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
/* file: sci_zoom_rect.c                                                  */
/* desc : interface for zoom_rect routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "getPropertyAssignedValue.h"
#include "axesScale.h"
#include "localization.h"
#include "Scierror.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "BuildObjects.h"

#include "JavaInteraction.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "createGraphicObject.h"

/*--------------------------------------------------------------------------*/
static int getZoomedObject(void* pvApiCtx, const char * fname);
static BOOL getZoomRect(void* pvApiCtx, const char * fname, int attribPos, double rect[4]);
/*--------------------------------------------------------------------------*/
/**
 * Get the [xmin, ymin, xmax, ymax] vector specified as input argument
 * @param fname name of the calling function for error messages
 * @param attribPos position of the argument within the rhs (1 or 2)
 * @param[out] rect retrieved rectangle
 * @return TRUE if the rect could be retrieved, false otherwise
 */
static BOOL getZoomRect(void* pvApiCtx, const char * fname, int attribPos, double rect[4])
{
    SciErr sciErr;
    int nbRow = 0;
    int nbCol = 0;
    int* piAddrstackPointer = NULL;
    double* stackPointer = NULL;
    int i = 0;
    double* rectVect = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, attribPos, &piAddrstackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position attribPos.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrstackPointer, &nbRow, &nbCol, &stackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, attribPos);
        return 1;
    }


    if (nbRow * nbCol != 4)
    {
        if (attribPos == 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 1, 4);
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 2, 4);
        }
        return FALSE;
    }

    rectVect = (stackPointer);
    for (i = 0; i < 4; i++)
    {
        rect[i] = rectVect[i];
    }

    return TRUE;

}
/*--------------------------------------------------------------------------*/
/**
 * Return the handle passed as input argument if one exists
 * @param fname of the function for errors
 * @return NULL if the input argument is not correct,
 *              the object to zoom otherwise
 */
static int getZoomedObject(void* pvApiCtx, const char * fname)
{
    SciErr sciErr;
    int nbRow  = 0;
    int nbCol = 0;
    int* piAddrstackPointer = NULL;
    long long* stackPointer = NULL;
    int iObjUID = 0;
    int iType = -1;
    int *piType = &iType;
    /* if a handle is specified it must be the first input argument */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &nbRow, &nbCol, &stackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 0;
    }


    /* check that there is only a single Figre or subwin */
    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Single handle expected.\n"), fname, 1);
        return 0;
    }

    iObjUID = getObjectFromHandle((long int) * stackPointer);

    if (iObjUID == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Figure or Axes handle expected.\n"), fname, 1);
        return 0;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType != __GO_FIGURE__ && iType != __GO_AXES__)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Figure or Axes handle expected.\n"), fname, 1);
        return 0;
    }

    return iObjUID;


}
/*--------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname, void *pvApiCtx)
{
    int iFigureUID = 0;
    int* piChildrenUID = NULL;
    int iChildrenCount = 0;
    int* childrencount = &iChildrenCount;
    int iHidden = 0;
    int *piHidden = &iHidden;
    int i = 0;

    CheckInputArgument(pvApiCtx, 0, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);
    if (nbInputArgument(pvApiCtx) == 0)
    {
        /* zoom_rect() */
        iFigureUID = getCurrentFigure();
        if (iFigureUID == 0)
        {
            iFigureUID = createNewFigureWithAxes();
        }
        startInteractiveZoom(iFigureUID);
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        /* zoom_rect([xmin,ymin,xmax,ymax]) or zoom_rect(handle) */
        /* with handle a figure or subwindow */
        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            int iZoomedObject = getZoomedObject(pvApiCtx, fname);
            if (iZoomedObject == 0)
            {
                return -1;
            }
            startInteractiveZoom(iZoomedObject);
        }
        else if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
        {
            double rect[4];
            if (getZoomRect(pvApiCtx, fname, 1, rect))
            {
                /* rectangle found */
                //int status = sciZoom2D(getCurrentSubWin(), rect);
                int status = 0;
                iFigureUID = getCurrentFigure();

                getGraphicObjectProperty(iFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);

                getGraphicObjectProperty(iFigureUID, __GO_CHILDREN__, jni_int_vector, (void **)&piChildrenUID);

                if (childrencount && piChildrenUID)
                {
                    for (i = 0; i < childrencount[0]; ++i)
                    {
                        getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                        if (iHidden == 0)
                        {
                            status = sciZoom2D(piChildrenUID[i], rect);
                        }
                    }
                }
                else
                {
                    status = SET_PROPERTY_ERROR;
                }
                if (status == SET_PROPERTY_ERROR)
                {
                    /* error on rectangle bounds */
                    Scierror(999, _("%s: Wrong value for input argument #%d: Specified bounds are not correct.\n"), fname, 1);
                    return -1;
                }
            }
            else
            {
                /* error on rectagle definition */
                return -1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Handle or vector of double expected.\n"), fname, 1);
            return 0;
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        /* zoom_rect(handle, [xmin,ymin,xmax,ymax]) */

        double rect[4];
        int iZoomedObject = 0;

        if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)) || (!checkInputArgumentType(pvApiCtx, 2, sci_matrix)))
        {
            Scierror(999, _("%s: Wrong type for input arguments: Handle or vector of double expected.\n"), fname);
            return -1;
        }

        iZoomedObject = getZoomedObject(pvApiCtx, fname);
        if (iZoomedObject == 0 || !getZoomRect(pvApiCtx, fname, 2, rect))
        {
            return -1;
        }

        if (sciZoomRect(iZoomedObject, rect) == SET_PROPERTY_ERROR)
        {
            /* error on rectangle bounds */
            Scierror(999, _("%s: Error on input argument #%d: Specified bounds are not correct.\n"), fname, 1);
            return -1;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/

