/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int sciZoom2D(char * subwinUID, const double zoomRect[4])
{
    if (subwinUID != NULL)
    {
        double* zoomBox = NULL;
        // add Z scale to data bounds.
        getGraphicObjectProperty(subwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **) &zoomBox);

        zoomBox[0] = zoomRect[0];
        zoomBox[1] = zoomRect[1];
        zoomBox[2] = zoomRect[2];
        zoomBox[3] = zoomRect[3];

        return sciZoom3D(subwinUID, zoomBox);
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
int sciZoom3D(char * subwinUID, const double zoomBox[6])
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

    if (!checkDataBounds(subwinUID, zoomBox3D[0], zoomBox3D[1], zoomBox3D[2],
                         zoomBox3D[3], zoomBox3D[4], zoomBox3D[5]))
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(subwinUID, __GO_ZOOM_BOX__, zoomBox3D, jni_double_vector, 6);

    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"zoom_box");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(subwinUID, __GO_ZOOM_ENABLED__, &zoomEnabled, jni_bool, 1);

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
int sciZoomRect(char* objUID, const double zoomRect[4])
{
    int iType = -1;
    int *piType = &iType;
    getGraphicObjectProperty(objUID, __GO_TYPE__, jni_int, (void **) &piType);
    if (iType == __GO_FIGURE__)
    {
        return sciFigureZoom2D(objUID, zoomRect);
    }
    else if (iType == __GO_AXES__)
    {
        return sciZoom2D(objUID, zoomRect);
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------------*/
int sciFigureZoom2D(char* figureUID, const double zoomRect[4])
{
    int i = 0;
    int childrenCount = 0;
    int* pChildrenCount = &childrenCount;

    char** children = NULL;

    getGraphicObjectProperty(figureUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &pChildrenCount);

    if ((pChildrenCount != NULL) && (childrenCount > 0))
    {
        getGraphicObjectProperty(figureUID, __GO_CHILDREN__, jni_string_vector, (void **) &children);
        for (i = 0; i < childrenCount; i++)
        {
          sciZoomRect(children[i], zoomRect);
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
BOOL checkDataBounds(char * subwinUID, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax)
{
    char logFlags[3];

    sciGetLogFlags(subwinUID, logFlags);

    /* check if there is not an inf within the values */
    /* since this has not any meaning */
    if (    !finite(xMin) || !finite(xMax)
         || !finite(yMin) || !finite(yMax)
         || !finite(zMin) || !finite(zMax) )
    {
        Scierror(999, "Error : data bounds values must be finite.");
        return FALSE;
    }

    /* check if the bounds are correct */
    /* allows equality with bounds since it is working */
    if ( xMin > xMax || yMin > yMax || zMin > zMax )
    {
        Scierror(999, "Error : Min and Max values for one axis do not verify Min <= Max.\n");
        return FALSE;
    }

    /* check for logflags that values are greater than 0 */
    if (   ( logFlags[0] == 'l' && xMin <= 0.0 )
        || ( logFlags[1] == 'l' && yMin <= 0.0 )
        || ( logFlags[2] == 'l' && zMin <= 0.0 ) )
    {
        Scierror(999, "Error: Bounds on axis must be strictly positive to use logarithmic mode.\n" );
        return FALSE;
    }

    return TRUE;
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom a single subwindow
 */
void sciUnzoomSubwin(char* subwinUID)
{
    char* parentFigure = NULL;
    int zoomEnabled = 0;

    getGraphicObjectProperty(subwinUID, __GO_PARENT_FIGURE__, jni_string, (void **) &parentFigure);

    if( subwinUID == NULL || parentFigure == NULL )
    {
        return;
    }

    setGraphicObjectProperty(subwinUID, __GO_ZOOM_ENABLED__, (void **) &zoomEnabled, jni_bool, 1);
}
/*------------------------------------------------------------------------------*/
/**
 * Unzoom all the subwindows contained in a figure
 */
void sciUnzoomFigure(char* figureUID)
{
  int iType = -1;
  int *piType = &iType;
  char** pstChildrenUID = NULL;

  int i = 0;
  int zoomEnabled = 0;
  int childrenCount = 0;
  int* piChildrenCount = &childrenCount;

  getGraphicObjectProperty(figureUID, __GO_CHILDREN__, jni_string_vector, (void **) &pstChildrenUID);
  getGraphicObjectProperty(figureUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

  if (piChildrenCount != NULL)
  {

    for (i = 0; i < childrenCount; i++)
    {
      getGraphicObjectProperty(pstChildrenUID[i], __GO_TYPE__, jni_int, (void **) &piType);
      if (iType == __GO_AXES__)
      {
          setGraphicObjectProperty(pstChildrenUID[i], __GO_ZOOM_ENABLED__, (void **) &zoomEnabled, jni_bool, 1);
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
void sciUnzoomArray(char* objectsUID[], int nbObjects)
{
  /* object type */
  int iType = -1;
  int *piType = &iType;
  int i = 0;
  for (i = 0; i < nbObjects; i++)
  {
    getGraphicObjectProperty(objectsUID[i], __GO_TYPE__, jni_int, (void **) &piType);
    if (iType == __GO_FIGURE__)
    {
      /* Unzoom all subwindows of the figure */
      sciUnzoomFigure(objectsUID[i]);
    }
    else if (iType == __GO_AXES__)
    {
      /* Unzoom the axes */
      sciUnzoomSubwin(objectsUID[i]);
    }

  }
}
/*--------------------------------------------------------------------------------*/
void updateSubwinScale(char * pSubwinUID)
{
    sciJavaUpdateSubwinScale(pSubwinUID);
}
/*------------------------------------------------------------------------------*/
void updateTextBounds(char * pTextUID)
{
    char* parentAxes = NULL;

    /* Update coordinate transformation if needed */
    getGraphicObjectProperty(pTextUID, __GO_PARENT_AXES__, jni_string, (void **) &parentAxes);
    updateSubwinScale(parentAxes);

    /* Compute the bounding box of the text */
    sciJavaUpdateTextBoundingBox(pTextUID);
}
/*------------------------------------------------------------------------------*/
