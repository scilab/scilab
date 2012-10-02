/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    Comment:
 *    This file contains all functions used to INTERACT with the graphic
 *    window (zoom by pressing button, callbacks...)
 --------------------------------------------------------------------------*/
#include <string.h>
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "JavaInteraction.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "HandleManagement.h"

#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"


/*-------------------------------------------------------------------------------------*/
static int moveObj(char* pobjUID, double displacement[], int displacementSize);
/*-------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
/* Objmove                                                                                         */
/* move a handle in the graphic window                                                             */
/*----------------------------------------------------------------------------------------*/

static int moveObj(char* pobjUID, double displacement[], int displacementSize)
{
    int i = 0;
    double x = displacement[0];
    double y = displacement[1];
    double z = (displacementSize == 3 ? displacement[2] : 0.0);

    int iNum = 0;
    int *piNum = &iNum;
    double* pdblData = NULL;
    double* pdblDataY = NULL;
    double* pdblDataZ = NULL;
    int iType = -1;
    int* piType = &iType;

    double* dataX = NULL;
    double* dataY = NULL;
    double* dataZ = NULL;

    int iNumX = 0;
    int iNumY = 0;
    int iNumZ = 0;
    int *piNumX = &iNumX;
    int *piNumY = &iNumY;
    int *piNumZ = &iNumZ;

    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    char **pstChildrenUID = NULL;

    // Get type
    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    // If a Figure, neither the object nor its children are moved.
    if (iType == __GO_FIGURE__)
    {
        Scierror(999, _("This object can not be moved.\n"));
        return -1;
    }

    // Iterate on children.
    getGraphicObjectProperty(pobjUID, __GO_CHILDREN_COUNT__, jni_int, (void**)&piChildrenCount);

    if (iChildrenCount != 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_CHILDREN__, jni_string_vector, (void **) &pstChildrenUID);
        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            moveObj(pstChildrenUID[i], displacement, displacementSize);
        }
    }

    // Arc.
    if (iType == __GO_ARC__)
    {
        getGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Fac3d.
    else if (iType == __GO_FAC3D__)
    {
        int iNumVPG = 0;
        int* piNumVPG = &iNumVPG;

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**)&piNumVPG);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**)&piNum);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

        for (i = 0; i < iNumVPG * iNum; i++)
        {
            dataX[i] += x;
            dataY[i] += y;
            dataZ[i] += z;
        }

        /* Model data has been updated by direct pointer access, trigger update within the renderer. */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNumVPG * iNum);

        return 0;
    }
    // Fec.
    else if (iType == __GO_FEC__)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**)&piNum);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void **)&pdblData);

        for (i = 0; i < iNum; i++)
        {
            pdblData[3 * i] += x;
            pdblData[3 * i + 1] += y;
            pdblData[3 * i + 2] += z;
        }

        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, pdblData, jni_double_vector, 3 * iNum);

        return 0;
    }
    // Grayplot.
    else if (iType == __GO_GRAYPLOT__)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        for (i = 0; i < iNumX; i++)
        {
            dataX[i] += x;
        }

        for (i = 0; i < iNumY; i++)
        {
            dataY[i] += y;
        }

        if (displacementSize == 3)
        {
            double zShift = 0.0;
            double* pdZshift = &zShift;

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_int, (void**)&pdZshift);
            zShift += z;
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, &zShift, jni_double, 1);
        }
        else
        {
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);
        }

        return 0;
    }
    // Matplot.
    else if (iType == __GO_MATPLOT__)
    {
        double zShift = 0.0;
        double* pdZShift = &zShift;
        //double* bounds = NULL;
        int type;
        int* itype = &type;

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_MATPLOT_TYPE__, jni_int, (void**)&itype);

        /* Only type 1 Matplot objects can be moved. */
        if (type == 1)
        {
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

            for (i = 0; i < iNumX; i++)
            {
                dataX[i] += x;
            }

            for (i = 0; i < iNumY; i++)
            {
                dataY[i] += y;
            }

            /* Bounds must be updated. */
            //getGraphicObjectProperty(pobjUID, __GO_MATPLOT_BOUNDS__, jni_double_vector, (void **)&bounds);

            //bounds[0] += x;
            //bounds[1] += x;
            //bounds[2] += y;
            //bounds[3] += y;

            if (displacementSize == 3)
            {
                getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_int, (void**)&pdZShift);
                zShift += z;
                setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, &zShift, jni_double, 1);
            }
            else
            {
                setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);
            }

            return 0;
        }
    }
    // Polyline.
    else if (iType == __GO_POLYLINE__)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**)&piNum);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        for (i = 0; i < iNum; i++)
        {
            dataX[i] += x;
            dataY[i] += y;
        }

        if (displacementSize == 3)
        {
            int zCoordinatesSet = 1;

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

            for (i = 0; i < iNum; i++)
            {
                dataZ[i] += z;
            }

            /* The z coordinates flag must be set explicitely for now. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_int, 1);

            /* Model data has been updated by direct pointer access, trigger update within the renderer. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNum);
        }
        else
        {
            /* Trigger update, see above. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNum);
        }

        return 0;
    }
    // Plot3d.
    else if (iType == __GO_PLOT3D__)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&piNumZ);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

        for (i = 0; i < iNumX; i++)
        {
            dataX[i] += x;
        }

        for (i = 0; i < iNumY; i++)
        {
            dataY[i] += y;
        }

        for (i = 0; i < iNumZ; i++)
        {
            dataZ[i] += z;
        }

        /*
         * Perform only one set call to trigger geometry data update within the renderer module.
         * Model data has been updated above by direct pointer access.
         */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNumZ);
        return 0;
    }
    // Champ.
    else if (iType == __GO_CHAMP__)
    {
        int *champDimensions = NULL;

        getGraphicObjectProperty(pobjUID, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void **)&champDimensions);

        getGraphicObjectProperty(pobjUID, __GO_BASE_X__, jni_double_vector, (void **)&pdblData);
        getGraphicObjectProperty(pobjUID, __GO_BASE_Y__, jni_double_vector, (void **)&pdblDataY);

        for (i = 0; i < champDimensions[0]; i++)
        {
            pdblData[i] += x;
        }

        for (i = 0; i < champDimensions[1]; i++)
        {
            pdblDataY[i] += y;
        }

        setGraphicObjectProperty(pobjUID, __GO_BASE_X__, pdblData, jni_double_vector, champDimensions[0]);
        setGraphicObjectProperty(pobjUID, __GO_BASE_Y__, pdblDataY, jni_double_vector, champDimensions[1]);

        if (displacementSize == 3)
        {
            getGraphicObjectProperty(pobjUID, __GO_BASE_Z__, jni_double_vector, (void **)&pdblDataZ);

            for (i = 0; i < champDimensions[0]*champDimensions[1]; i++)
            {
                pdblDataZ[i] += z;
            }

            setGraphicObjectProperty(pobjUID, __GO_BASE_Z__, pdblDataZ, jni_double_vector, champDimensions[0]*champDimensions[1]);
        }
        return 0;
    }
    // Label.
    else if (iType == __GO_LABEL__)
    {
        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_POSITION__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Rectangle.
    else if (iType == __GO_RECTANGLE__)
    {
        getGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Segs.
    else if (iType == __GO_SEGS__)
    {
        double* pdblDirection = NULL;

        getGraphicObjectProperty(pobjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNum);
        getGraphicObjectProperty(pobjUID, __GO_BASE__, jni_double_vector, (void **)&pdblData);
        getGraphicObjectProperty(pobjUID, __GO_DIRECTION__, jni_double_vector, (void **)&pdblDirection);

        for (i = 0; i < iNum; i++)
        {
            pdblData[3 * i] += x;
            pdblData[3 * i + 1] += y;
            pdblData[3 * i + 2] += z;
        }

        for (i = 0; i < iNum; i++)
        {
            pdblDirection[3 * i] += x;
            pdblDirection[3 * i + 1] += y;
            pdblDirection[3 * i + 2] += z;
        }

        setGraphicObjectProperty(pobjUID, __GO_BASE__, pdblData, jni_double_vector, 3 * iNum);
        setGraphicObjectProperty(pobjUID, __GO_DIRECTION__, pdblDirection, jni_double_vector, 3 * iNum);

        return 0;
    }
    // Text.
    else if (iType == __GO_TEXT__)
    {
        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_POSITION__, pdblData, jni_double_vector, 3);

        return 0;
    }

    if (iType == __GO_AXES__ || iType == __GO_COMPOUND__)
    {
        // Children already moved: Done.
        return 0;
    }

    // Default error.
    Scierror(999, _("This object can not be moved.\n"));
    return -1;
    /*
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        // X
        for (i = 0 ; i < iNumX ; ++i)
        {
            dataX[i] += x;
        }
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, dataX, jni_double_vector, iNumX);

        // Y
        for (i = 0 ; i < iNumY ; ++i)
        {
            dataY[i] += y;
        }
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);

        // Z if needed
        if (displacementSize == 3)
        {
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&piNumZ);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);
            for (i = 0 ; i < iNumZ ; ++i)
            {
                dataZ[i] += z;
            }
        }

        return 0;
    */

    return 0;
}

int Objmove (char *pobjUID, double d[], int m, BOOL opt)
{
    return moveObj(pobjUID, d, m);
}

/*---------------------------------------------------------------------------------*/
void showWindow(char * pFigureUID)
{
    // @TODO: Call SwingView to raise window.
}

