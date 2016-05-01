/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - 2004 - INRIA - Serge Steer
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "GetProperty.h"
#include "Scierror.h"
#include "InitObjects.h"
#include "CurrentFigure.h"
#include "GetJavaProperty.h"
#include "BasicAlgos.h"
#include "localization.h"
#include "Axes.h"
#include "api_scilab.h"
#include "HandleManagement.h"

#include "sci_malloc.h" /* MALLOC */

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "FigureModel.h"

/**sciGetNumColors
* This function gets the number of the color defined in colormap
*/
int sciGetNumColors (int iObjUID)
{
    if (iObjUID)
    {
        int iParentFigure = 0;
        int* piParent = &iParentFigure;
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getGraphicObjectProperty(iObjUID, __GO_PARENT_FIGURE__, jni_int, (void **)&piParent);
        getGraphicObjectProperty(iParentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

        return iNumColors;
    }
    return -1;
}

/****************************************** TEXT ******************************

/**
* Checks if a text object is empty #rows*#columns==0 or #rows*#columns==1 and entry is  zero length
* This function has been adapted to the MVC: its parameter's type has been changed from sciPointObj to char*
* (MVC identifier), the reason being that it is exclusively used to get the properties of Label objects,
* which can be only be known by their MVC identifiers at the present moment.
*/
//BOOL sciisTextEmpty(sciPointObj * pobj)

BOOL sciisTextEmpty(int iObjUID)
{
    int nbElements = 0;
    int* dimensions = NULL;

    getGraphicObjectProperty(iObjUID, __GO_TEXT_ARRAY_DIMENSIONS__, jni_int_vector, (void **)&dimensions);

    if (dimensions == NULL)
    {
        return TRUE;
    }

    nbElements = dimensions[0] * dimensions[1];

    releaseGraphicObjectProperty(__GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    if (nbElements == 0)
    {
        return TRUE;
    }

    if (nbElements == 1)
    {
        char** textMatrix = NULL;
        BOOL ret = FALSE;
        getGraphicObjectProperty(iObjUID, __GO_TEXT_STRINGS__, jni_string_vector, (void **) &textMatrix);

        if (textMatrix[0] == NULL)
        {
            ret = TRUE;
        }
        else if (strcmp(textMatrix[0], "") == 0)
        {
            /* empty string */
            ret = TRUE;
        }

        releaseGraphicObjectProperty(__GO_TEXT_STRINGS__, textMatrix, jni_string_vector, nbElements);
        return ret;
    }

    return FALSE;
}

/*--------------------------------------------------------------------------*/

/**sciGetPoint
* returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
*/

/**MAJ pour le 3D DJ.Abdemouche 2003**/
double *sciGetPoint(int iObjUID, int *numrow, int *numcol)
{
    int iType = -1;
    int *piType = &iType;
    double *tab = NULL;
    int i = 0;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    /*
     * Object type determined by string comparisons
     * Required as we have no better way to do this for the moment
     */

    switch (iType)
    {
        case __GO_FIGURE__ :
        {
            int* figurePosition = NULL;
            int* axesSize = NULL;

            *numrow = 2;
            *numcol = 2;
            if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_int_vector, (void **)&figurePosition);
            getGraphicObjectProperty(iObjUID, __GO_AXES_SIZE__, jni_int_vector, (void **)&axesSize);

            tab[0] = (double) figurePosition[0];
            tab[1] = (double) figurePosition[1];
            tab[2] = (double) axesSize[0];
            tab[3] = (double) axesSize[1];

            return tab;
        }
        case __GO_POLYLINE__ :
        {
            int iParentAxes = 0;
            int* piAxes = &iParentAxes;
            double* dataX = NULL;
            double* dataY = NULL;
            double* dataZ = NULL;
            int iTmp = 0;
            int* piTmp = &iTmp;
            int iView = 0;
            int *piView = &iView;

            /*
             * Testing whether data properties exist for this object
             * is currently done only for this property. The type comparison already
             * ensures that this is the case, though doing so is awkward.
             */
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&piTmp);

            if (piTmp == NULL)
            {
                *numrow = -2;
                *numcol = -2;
                return NULL;
            }

            *numrow = iTmp;

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, jni_int, (void**)&piTmp);

            if (iTmp)
            {
                *numcol = 3;
            }
            else
            {
                *numcol = 2;
            }

            if ((*numrow) * (*numcol) == 0)
            {
                /* empty data, no warnings */
                *numrow = 0;
                *numcol = 0;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piAxes);
            getGraphicObjectProperty(iParentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

            if (*numcol == 2 && iView)
            {
                *numcol = (*numcol) + 1; /* colonne de 0. a prendre en compte / afficher => numcol+1*/
                if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
                {
                    *numrow = -1;
                    *numcol = -1;
                    return NULL;
                }

                for (i = 0 ; i < *numrow ; i++)
                {
                    tab[i] = dataX[i];
                    tab[*numrow + i] = dataY[i];
                    tab[(2 * (*numrow)) + i] = 0.;
                }
            }
            else
            {
                if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
                {
                    *numrow = -1;
                    *numcol = -1;
                    return NULL;
                }

                if (*numcol == 3)
                {
                    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);
                }

                for (i = 0 ; i < *numrow ; i++)
                {
                    tab[i] = dataX[i];
                    tab[*numrow + i] = dataY[i];
                    if (*numcol == 3)
                    {
                        tab[(2 * (*numrow)) + i] = dataZ[i];
                    }
                }

            }
            return tab;
        }
        case __GO_RECTANGLE__ :
        {
            int iParentAxes = 0;
            int* piAxes = &iParentAxes;
            double* upperLeftPoint = NULL;
            double width = 0.0;
            double *pdblWidth = &width;
            double height = 0.0;
            double *pdblHeight = &height;
            int iView = 0;
            int *piView = &iView;

            *numrow = 1;

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piAxes);
            getGraphicObjectProperty(iParentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            *numcol = iView ? 5 : 4;

            if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&upperLeftPoint);

            getGraphicObjectProperty(iObjUID, __GO_WIDTH__, jni_double, (void**)&pdblWidth);
            getGraphicObjectProperty(iObjUID, __GO_HEIGHT__, jni_double, (void **)&pdblHeight);

            tab[0] = upperLeftPoint[0];
            tab[1] = upperLeftPoint[1];

            if (iView)
            {
                tab[2] = upperLeftPoint[2];
                tab[3] = width;
                tab[4] = height;
            }
            else
            {
                tab[2] = width;
                tab[3] = height;
            }
            return (double*)tab;
        }
        case __GO_ARC__ :
        {
            int iParentAxes = 0;
            int* piAxes = &iParentAxes;
            double* upperLeftPoint = NULL;
            double width = 0.0;
            double *pdblWidth = &width;

            double height = 0.;
            double *pdblHeight = &height;

            double startAngle = 0.;
            double *pdblStartAngle = &startAngle;
            double endAngle = 0.;
            double *pdblEndAngle = &endAngle;
            int view = 0;
            int *piView = &view;

            *numrow = 1;

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piAxes);
            getGraphicObjectProperty(iParentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            *numcol = view ? 7 : 6;

            if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&upperLeftPoint);

            getGraphicObjectProperty(iObjUID, __GO_WIDTH__, jni_double, (void **)&pdblWidth);
            getGraphicObjectProperty(iObjUID, __GO_HEIGHT__, jni_double, (void **)&pdblHeight);

            getGraphicObjectProperty(iObjUID, __GO_START_ANGLE__, jni_double, (void **)&pdblStartAngle);
            getGraphicObjectProperty(iObjUID, __GO_END_ANGLE__, jni_double, (void **)&pdblEndAngle);

            tab[0] = upperLeftPoint[0];
            tab[1] = upperLeftPoint[1];

            if (view)
            {
                tab[2] = upperLeftPoint[2];
                tab[3] = width;
                tab[4] = height;
                tab[5] = RAD2DEG(startAngle);
                tab[6] = RAD2DEG(endAngle);
            }
            else
            {
                tab[2] = width;
                tab[3] = height;
                tab[4] = RAD2DEG(startAngle);
                tab[5] = RAD2DEG(endAngle);
            }

            return (double*)tab;
        }
        case __GO_COMPOUND__ :
        {
            return (double*)NULL;
        }
        case __GO_TEXT__ :
        {
            int iParentAxes = 0;
            int* piAxes = &iParentAxes;
            double* textPosition = NULL;
            int iView = 0;
            int* piView = &iView;

            *numrow = 1;

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piAxes);
            getGraphicObjectProperty(iParentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            *numcol = iView ? 3 : 2;

            if ((tab = (double*)CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **)&textPosition);

            tab[0] = textPosition[0];
            tab[1] = textPosition[1];

            if (iView)
            {
                tab[2] = textPosition[2];
            }

            return (double*)tab;
        }
        case __GO_SEGS__ :
        {
            int iView = 0;
            int* piView = &iView;
            int iNumArrows = 0;
            int* piNumArrows = &iNumArrows;
            int iParentAxes = 0;
            int* piAxes = &iParentAxes;
            double* arrowBases = NULL;
            double* arrowDirections = NULL;

            getGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNumArrows);
            *numrow = iNumArrows;

            /* only two coordinates are displayed if the axe is in 2d
            and the z coordinates has never been modified */

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piAxes);

            getGraphicObjectProperty(iParentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            if (iView == 0) // 2-D
            {
                *numcol = 2;
            }
            else // 3-D
            {
                *numcol = 3;
            }

            if ((tab = (double*)CALLOC(2 * (*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_BASE__, jni_double_vector, (void **)&arrowBases);
            getGraphicObjectProperty(iObjUID, __GO_DIRECTION__, jni_double_vector, (void **)&arrowDirections);

            for (i = 0; i < *numrow; i++)
            {
                tab[2 * i] = arrowBases[3 * i];
                tab[2 * i + 1] = arrowDirections[3 * i];
                tab[2 * (*numrow) + 2 * i] = arrowBases[3 * i + 1];
                tab[2 * (*numrow) + 2 * i + 1] = arrowDirections[3 * i + 1];
                if (*numcol == 3)
                {
                    tab[4 * (*numrow) + 2 * i] = arrowBases[3 * i + 2];
                    tab[4 * (*numrow) + 2 * i + 1] = arrowDirections[3 * i + 2];
                }
            }

            /* There are twice as many points as arrows (2 endpoints) */
            *numrow = 2 * (*numrow);

            return (double*)tab;
        }
        case __GO_FAC3D__ :
        case __GO_PLOT3D__ :
        {
            *numrow = -1;
            *numcol = -1;
            return (double*) NULL;
        }
        case __GO_MATPLOT__ :
        {
            int nx = 0;
            int *piNx = &nx;
            int ny = 0;
            int *piNy = &ny;
            double* data;

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNx);

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNy);

            /* The z data matrix has (ny-1) rows and (nx-1) cols */
            nx = nx - 1;
            ny = ny - 1;

            *numrow = ny;
            *numcol = nx;

            if ((tab = (double*)CALLOC(nx * ny, sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return (double*)NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&data);

            for (i = 0; i < nx * ny; i++)
            {
                tab[i] = data[i];
            }

            return (double*)tab;
        }
        case __GO_FEC__ :
        {
            double* coordinates = NULL;
            double* values = NULL;
            int iTmp = 0;
            int* piTmp = &iTmp;

            *numcol = 3;

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**)&piTmp);
            *numrow = iTmp;

            if ((tab = (double*)CALLOC(*numrow * 3, sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return (double*)NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void **)&coordinates);
            getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void **)&values);

            for (i = 0; i < *numrow; i++)
            {
                tab[i] = coordinates[3 * i];
                tab[*numrow + i] = coordinates[3 * i + 1];
                tab[*numrow * 2 + i] = values[i];

            }

            return (double*)tab;
        }
        case __GO_LEGEND__ :
        case __GO_AXES__ :
        case __GO_LABEL__ :
        default :
        {
            *numrow = -2;
            *numcol = -2;
            return (double*)NULL;
        }
    }
#if 0
case SCI_UIMENU:
#endif
    return (double*)NULL;
}

/*-------------------------------------------------------------------------------*/
BOOL sciGetLegendDefined(int iObjUID)
{
    int iXLabelId = 0;
    int* piX = &iXLabelId;
    int iYLabelId = 0;
    int* piY = &iYLabelId;
    int iZLabelId = 0;
    int* piZ = &iZLabelId;

    if (iObjUID == 0)
    {
        return FALSE;
    }

    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_LABEL__, jni_int, (void **)&piX);
    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LABEL__, jni_int, (void **)&piY);
    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LABEL__, jni_int, (void **)&piZ);

    /* get the text size of labels */
    if (sciisTextEmpty(iXLabelId) &&
            sciisTextEmpty(iYLabelId) &&
            sciisTextEmpty(iZLabelId))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*-----------------------------------------------------------------------------------*/
/**
* Copy the logflags of a subwin to an array of size 3.
*/
void sciGetLogFlags(int iObjUID, char flags[3])
{
    int const logflagPropertyNames[3] = {__GO_X_AXIS_LOG_FLAG__, __GO_Y_AXIS_LOG_FLAG__, __GO_Z_AXIS_LOG_FLAG__};
    int i = 0;
    int iLogFlag = 0;
    int* piLogFlag = &iLogFlag;

    for (i = 0; i < 3; i++)
    {
        getGraphicObjectProperty(iObjUID, logflagPropertyNames[i], jni_bool, (void **)&piLogFlag);

        if (piLogFlag == NULL)
        {
            printSetGetErrorMessage("log_flags");
            return;
        }

        if (iLogFlag)
        {
            flags[i] = 'l';
        }
        else
        {
            flags[i] = 'n';
        }
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert user coordinates to user cooordinates (2D).
* @param pObjUID subwindow identifier
* @param userCoord3D user coordinates
* @param userCoords2D result in user coordinates in the default 2D plane.
*/
void sciGet2dViewCoordinate(int iObjUID, const double userCoords3D[3], double userCoords2D[2])
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        sciGetJava2dViewCoordinates(iObjUID, userCoords3D, userCoords2D);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        userCoords2D[0] = 0.0;
        userCoords2D[1] = 0.0;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert pixel coordinates to 2D view coordinate
* @param pObjUID subwindow identifier
* @param userCoord pixel coordinates
* @param userCoords2D user coordinates in default 2D plane
*/
void sciGet2dViewCoordFromPixel(int iObjUID, const int pixelCoords[2], double userCoords2D[2])
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        sciGetJava2dViewCoordFromPixel(iObjUID, pixelCoords, userCoords2D);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        userCoords2D[0] = 0.0;
        userCoords2D[1] = 0.0;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert 2d view coordinates to pixel coordinates
* @param pObjUID subwindow identifier
* @param userCoords2D coordinates in the default 2D plane
* @param pixelsCoords pixel coordinates
*/
void sciGet2dViewPixelCoordinates(int iObjUID, const double userCoords2D[2], int pixelCoords[2])
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        /* create a 3d user coord */
        double userCoord3D[3] = {userCoords2D[0], userCoords2D[1], 0.0};
        sciGetJava2dViewPixelCoordinates(iObjUID, userCoord3D, pixelCoords);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        pixelCoords[0] = -1;
        pixelCoords[1] = -1;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the viewing area of a subwindow acoording to its axes scale and margins
* result is in pixels
*/
void sciGetViewingArea(int iObjUID, int * xPos, int * yPos, int * width, int * height)
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (iType == __GO_AXES__)
    {
        sciGetJavaViewingArea(iObjUID, xPos, yPos, width, height);
    }
    else
    {
        *xPos = -1;
        *yPos = -1;
        *width = -1;
        *height = -1;
        Scierror(999, _("Only axes handles have a viewing area."));
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Print the message "This object has no xxx property." in Scilab.
*/
void printSetGetErrorMessage(const char * propertyName)
{
    Scierror(999, _("This object has no %s property.\n"), propertyName);
}
/*----------------------------------------------------------------------------------*/

