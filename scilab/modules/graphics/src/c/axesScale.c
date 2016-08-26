/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: axesScale.c                                                      */
/* desc : Contains functions to compute scale changes in a specific       */
/*        subwindow.                                                      */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "axesScale.h"
#include "math_graphics.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "SetJavaProperty.h"
#include "Interaction.h"
#include "JavaInteraction.h"
#include "HandleManagement.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------------*/
/**
 * Specify new zoom box for a subwin object.
 * @param subwinUID the subwin's identifier.
 * @param zoomRect vector [xMin, yMin, xMax, yMax].
 */
int sciZoom2D(int iSubwinUID, const double zoomRect[4])
{
    if (iSubwinUID != 0)
    {
        double* zoomBox = NULL;
        // add Z scale to data bounds.
        getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **) &zoomBox);

        if (zoomBox)
        {
            zoomBox[0] = zoomRect[0];
            zoomBox[1] = zoomRect[1];
            zoomBox[2] = zoomRect[2];
            zoomBox[3] = zoomRect[3];
            return sciZoom3D(iSubwinUID, zoomBox);
        }
        else
        {
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------------*/
/**
 * Specify a new zoom box for a subwin object
 * @param subwinUID the subwin's identifier.
 * @param zoomBox vector [xMin, yMin, xMax, yMax, zMin, zMax].
 */
int sciZoom3D(int iSubwinUID, const double zoomBox[6])
{
    BOOL status = FALSE;
    int zoomEnabled = 1;

    // convert zoomBox to [xMin, xMax, yMin, yMax, zMin, zMax]
    double zoomBox3D[6];
    zoomBox3D[0] = zoomBox[0];
    zoomBox3D[1] = zoomBox[2];
    zoomBox3D[2] = zoomBox[1];
    zoomBox3D[3] = zoomBox[3];
    zoomBox3D[4] = zoomBox[4];
    zoomBox3D[5] = zoomBox[5];

    if (!checkDataBounds(iSubwinUID, zoomBox3D[0], zoomBox3D[1], zoomBox3D[2],
                         zoomBox3D[3], zoomBox3D[4], zoomBox3D[5]))
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iSubwinUID, __GO_ZOOM_BOX__, zoomBox3D, jni_double_vector, 6);

    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "zoom_box");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iSubwinUID, __GO_ZOOM_ENABLED__, &zoomEnabled, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------------*/
int sciZoomRect(int iObjUID, const double zoomRect[4])
{
    int iType = -1;
    int *piType = &iType;
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **) &piType);
    if (iType == __GO_FIGURE__)
    {
        return sciFigureZoom2D(iObjUID, zoomRect);
    }
    else if (iType == __GO_AXES__)
    {
        return sciZoom2D(iObjUID, zoomRect);
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------------*/
int sciFigureZoom2D(int iFigureUID, const double zoomRect[4])
{
    int i = 0;
    int childrenCount = 0;
    int* pChildrenCount = &childrenCount;

    int* piChildren = NULL;

    getGraphicObjectProperty(iFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &pChildrenCount);

    if ((pChildrenCount != NULL) && (childrenCount > 0))
    {
        getGraphicObjectProperty(iFigureUID, __GO_CHILDREN__, jni_int_vector, (void **) &piChildren);
        for (i = 0; i < childrenCount; i++)
        {
            sciZoomRect(piChildren[i], zoomRect);
        }
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------------*/
/**
 * Check if the following data bounds can be used as new data bounds for the subwin object
 * @param subwinUID the subwin's identifier.
 * @param the lower x bound.
 * @param the upper x bound.
 * @param the lower y bound.
 * @param the upper y bound.
 * @param the lower z bound.
 * @param the upper z bound.
 * @return TRUE if values can be used, false otherwise
 */
BOOL checkDataBounds(int iSubwinUID, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax)
{
    char logFlags[3];

    sciGetLogFlags(iSubwinUID, logFlags);

    /* check if there is not an inf within the values */
    /* since this has not any meaning */
    if (   !finite(xMin) || !finite(xMax)
            || !finite(yMin) || !finite(yMax)
            || !finite(zMin) || !finite(zMax))
    {
        Scierror(999, "Error : data bounds values must be finite.");
        return FALSE;
    }

    /* check if the bounds are correct */
    /* allows equality with bounds since it is working */
    if (xMin > xMax || yMin > yMax || zMin > zMax)
    {
        Scierror(999, "Error : Min and Max values for one axis do not verify Min <= Max.\n");
        return FALSE;
    }

    /* check for logflags that values are greater than 0 */
    if (  (logFlags[0] == 'l' && xMin <= 0.0)
            || (logFlags[1] == 'l' && yMin <= 0.0)
            || (logFlags[2] == 'l' && zMin <= 0.0))
    {
        Scierror(999, "Error: Bounds on axis must be strictly positive to use logarithmic mode.\n");
        return FALSE;
    }

    return TRUE;
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a single subwindow
 */
void sciUnzoomSubwin(int iSubwinUID)
{
    int iParentFigure = 0;
    int* piParent = &iParentFigure;
    int zoomEnabled = 0;

    getGraphicObjectProperty(iSubwinUID, __GO_PARENT_FIGURE__, jni_int, (void **) &piParent);

    if (iSubwinUID == 0 || iParentFigure == 0)
    {
        return;
    }

    setGraphicObjectProperty(iSubwinUID, __GO_ZOOM_ENABLED__, (void **) &zoomEnabled, jni_bool, 1);
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom all the subwindows contained in a figure
 */
void sciUnzoomFigure(int iFigureUID)
{
    int iType = -1;
    int *piType = &iType;
    int* piChildrenUID = NULL;

    int i = 0;
    int zoomEnabled = 0;
    int childrenCount = 0;
    int* piChildrenCount = &childrenCount;

    getGraphicObjectProperty(iFigureUID, __GO_CHILDREN__, jni_int_vector, (void **) &piChildrenUID);
    getGraphicObjectProperty(iFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

    if (piChildrenCount != NULL)
    {

        for (i = 0; i < childrenCount; i++)
        {
            getGraphicObjectProperty(piChildrenUID[i], __GO_TYPE__, jni_int, (void **) &piType);
            if (iType == __GO_AXES__)
            {
                setGraphicObjectProperty(piChildrenUID[i], __GO_ZOOM_ENABLED__, (void **) &zoomEnabled, jni_bool, 1);
            }
        }
    }
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a set of subwindows and figures
 * @param objectsUID array of figure and subwindow objects id.
 * @param nbObjects number of objects.
 */
void sciUnzoomArray(int* piObjUID, int nbObjects)
{
    /* object type */
    int iType = -1;
    int *piType = &iType;
    int i = 0;
    for (i = 0; i < nbObjects; i++)
    {
        getGraphicObjectProperty(piObjUID[i], __GO_TYPE__, jni_int, (void **) &piType);
        if (iType == __GO_FIGURE__)
        {
            /* Unzoom all subwindows of the figure */
            sciUnzoomFigure(piObjUID[i]);
        }
        else if (iType == __GO_AXES__)
        {
            /* Unzoom the axes */
            sciUnzoomSubwin(piObjUID[i]);
        }

    }
}
/*--------------------------------------------------------------------------------*/
void updateSubwinScale(int iSubwinUID)
{
    sciJavaUpdateSubwinScale(iSubwinUID);
}
/*------------------------------------------------------------------------------*/
void updateTextBounds(int iTextUID)
{
    int iParentAxes = 0;
    int* piParent = &iParentAxes;

    /* Update coordinate transformation if needed */
    getGraphicObjectProperty(iTextUID, __GO_PARENT_AXES__, jni_int, (void **) &piParent);
    updateSubwinScale(iParentAxes);

    /* Compute the bounding box of the text */
    sciJavaUpdateTextBoundingBox(iTextUID);
}
/*------------------------------------------------------------------------------*/
