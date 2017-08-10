/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    -This file contains all functions used to SET the properties of graphics
 *    objects.
 *    - there are two kinds of functions : the sciInit*** which only set an object
 *      property and sciSet*** which do the same work but check before if it is
 *      necessary to do so.
 *    - The returning status of each functions is as follow :
 *      0 for correct execution, 1 if no change was made
 *      and -1 if an error occurred.
 --------------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>

#include "os_string.h"

#include "SetProperty.h"
#include "GetProperty.h"
#include "GetJavaProperty.h"
#include "InitObjects.h"
#include "BuildObjects.h"
#include "math_graphics.h"
#include "Scierror.h"
#include "CurrentFigure.h"
#include "BasicAlgos.h"
#include "FigureList.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"
#include "sciprint.h"

#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FigureModel.h"
#include "AxesModel.h"

/**sciSetLineWidth
 * Sets the line width
 */
int sciSetLineWidth (int iObjUID, double linewidth)
{
    BOOL status = FALSE;

    if (linewidth < 0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be greater or equal to %d.\n"), "thickness", 0);
        return -1;
    }
    else
    {
        status = setGraphicObjectProperty(iObjUID, __GO_LINE_THICKNESS__, &linewidth, jni_double, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }
    printSetGetErrorMessage("thickness");
    return -1;
}

/**sciSetLineStyle
 * Sets the line style
 */
int sciSetLineStyle(int iObjUID, int linestyle)
{
    BOOL status = FALSE;
    if (linestyle < 0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be greater or equal to %d.\n"), "line_style", 0);
        return -1;
    }
    else
    {
        status = setGraphicObjectProperty(iObjUID, __GO_LINE_STYLE__, &linestyle, jni_int, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }

    printSetGetErrorMessage("line_style");
    return -1;
}

int sciSetMarkSize(int iObjUID, int *markSizes, int numMarkSizes)
{
    BOOL status;
    int k;

    if ( markSizes == NULL || numMarkSizes < 1 )
    {
        Scierror(999, _("Wrong value for '%s' property: Number of mark sizes %d.\n"), "mark_size", numMarkSizes);
        return -1;
    }

    // check values >= 0
    for ( k = 0; k < numMarkSizes; ++k )
    {
        if ( markSizes[k] < 0 )
        {
            Scierror(999, _("Wrong value for '%s' property: Must be greater or equal to %d.\n"), "mark_size", 0);
            return -1;
        }
    }

    if ( numMarkSizes == 1 )
    {
        status = setGraphicObjectProperty(iObjUID, __GO_MARK_SIZE__, &markSizes[0], jni_int, numMarkSizes);
    }
    else
    {
        status = setGraphicObjectProperty(iObjUID, __GO_MARK_SIZES__, markSizes, jni_int_vector, numMarkSizes);
    }

    if (status == TRUE)
    {
        return 0;
    }

    printSetGetErrorMessage("mark_size");
    return -1;
}

int sciSetMarkOffset(int iObjUID, int offset)
{
    if (offset < 0)
    {
        Scierror(999, _("The mark offset must be greater or equal than %d.\n"), 0);
        return -1;
    }
    else
    {
        BOOL status = setGraphicObjectProperty(iObjUID, __GO_MARK_OFFSET__, &offset, jni_int, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }

    printSetGetErrorMessage("mark_offset");
    return -1;
}

int sciSetMarkStride(int iObjUID, int stride)
{
    if (stride < 1)
    {
        Scierror(999, _("The mark stride must be greater or equal than %d.\n"), 1);
        return -1;
    }
    else
    {
        BOOL status = setGraphicObjectProperty(iObjUID, __GO_MARK_STRIDE__, &stride, jni_int, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }

    printSetGetErrorMessage("mark_stride");
    return -1;
}

/**sciSetText
 * Sets the Text in TEXT, TITLE or LEGEND
 * @param char * pobjUID: the pointer to the entity
 * @param char *text[] : the text which has to be put
 * @param int nbRow : the number of row of the text matrix
 * @param int nbCol : the number of col of the text matrix
 * @return  0 if OK, -1 if not
 */
int sciSetText (int iObjUID, char ** text, int nbRow, int nbCol)
{
    int dimensions[2];
    BOOL status = FALSE;

    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(text, nbRow, nbCol);

    dimensions[0] = nbRow;
    dimensions[1] = nbCol;

    status = setGraphicObjectProperty(iObjUID, __GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    if (status != TRUE)
    {
        printSetGetErrorMessage("text");
        return -1;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, dimensions[0] * dimensions[1]);

    if (status == TRUE)
    {
        return 0;
    }
    else
    {
        printSetGetErrorMessage("text");
        return -1;
    }
}

/**sciSetDefaultValue
 * Sets the default values of figure properties
 * (graphics context, graphics mode, and graphics font)
 * @author Djalel ABDEMOUCHE
 */
int
sciSetDefaultValues (void)
{
    // FIXME : To be removed
    return 0;
}

/*-------------------------------------------------------------------------------*/

/**sciSetPoint
 * sets points of the entity, and a pointer to the number of points
 */
/** MAJ pour le 3D DJ.Abdemouche 2003**/
int sciSetPoint(int iObjUID, double *tab, int *numrow, int *numcol)
{
    int iType = -1;
    int *piType = &iType;
    int i = 0, n1 = 0;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    /*
     * switch over sciGetEntityType replaced by object type string comparisons
     * Still required as we have no better way to do this for the moment
     */
    switch (iType)
    {
        case __GO_POLYLINE__ :
        {
            BOOL result;
            int numElementsArray[2];
            int zCoordinatesSet;

            if ((*numcol != 3) && (*numcol != 2) && (*numcol != 0))
            {
                Scierror(999, _("Number of columns must be %d (%d if %s coordinate).\n"), 2, 3, "z");
                return -1;
            }

            /*
             * The coordinates array is re-allocated (if required) within the data model, testing whether
             * the new number of points is different in order to free/re-allocate is not needed anymore.
             * The 0-element array case is managed by the data model as well.
             */

            if (*numcol == 0)
            {
                n1 = 0;
            }
            else
            {
                n1 = *numrow;
            }

            /* The first element must be equal to 1 for a Polyline, the second is the polyline's number of vertices */
            numElementsArray[0] = 1;
            numElementsArray[1] = n1;

            /* Resizes the data coordinates array if required */
            result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

            /*
             * For now, the FALSE return value corresponds to a failed memory allocation,
             * which does not allow to discriminate between the failed allocation and non-existing
             * property conditions.
             */
            if (result == FALSE)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
                return -1;
            }

            if (*numcol > 0)
            {
                setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_X__, tab, jni_double_vector, n1);
                setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Y__, &tab[n1], jni_double_vector, n1);

                if (*numcol == 3)
                {
                    setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Z__, &tab[2 * n1], jni_double_vector, n1);
                    zCoordinatesSet = 1;
                }
                else
                {
                    zCoordinatesSet = 0;
                }

                /* Required for now to indicate that the z coordinates have been set or not */
                setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_int, 1);
            }
            else
            {
                // to be sure that Java will be warned
                setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL__, NULL, jni_double_vector, 0);
            }

            return 0;
        }
        case __GO_RECTANGLE__ :
        {
            double* currentUpperLeftPoint = NULL;
            double upperLeftPoint[3];
            int widthIndex = 2;
            int size = *numrow **numcol;

            if (size != 5 && size != 4)
            {
                Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"), 4, 5, "z");
                return -1;
            }

            upperLeftPoint[0] = tab[0];
            upperLeftPoint[1] = tab[1];

            if (size == 5)
            {
                upperLeftPoint[2] = tab[2];
                widthIndex = 3;
            }
            else
            {
                /*
                 * Needed in order to set the z coordinate if size == 4
                 * Being able to set only the point's x and y coordinates values would avoid doing this.
                 */
                getGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&currentUpperLeftPoint);
                upperLeftPoint[2] = currentUpperLeftPoint[2];
            }

            setGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

            /* check that the height and width are positive */
            if (tab[widthIndex] < 0.0 || tab[widthIndex + 1] < 0.0)
            {
                Scierror(999, "Width and height must be positive.\n");
                return -1;
            }

            setGraphicObjectProperty(iObjUID, __GO_WIDTH__, &tab[widthIndex], jni_double, 1);
            setGraphicObjectProperty(iObjUID, __GO_HEIGHT__, &tab[widthIndex + 1], jni_double, 1);

            return 0;
        }
        case __GO_ARC__ :
        {
            double startAngle = 0.;
            double endAngle = 0.;
            double upperLeftPoint[3];
            double width = 0.;
            double height = 0.;
            double* currentUpperLeftPoint = NULL;
            int size = 0;

            size = *numrow **numcol;

            if ((size != 7) && (size != 6))
            {
                Scierror(999, _("Number of elements must be %d (%d if z coordinate)\n"), 6, 7);
                return -1;
            }

            upperLeftPoint[0] = tab[0];
            upperLeftPoint[1] = tab[1];

            /*
             * Setting the data has been made consistent with how it is done for the Rectangle:
             * it takes into account the size of the input array instead of the parent Axes'
             * view property. Using the latter led to incorrectly set values when size and view
             * were not corresponding (for example when size==7, and view==2d).
             */
            if (size == 7)
            {
                upperLeftPoint[2] = tab[2];
                width = tab[3];
                height = tab[4];
                startAngle = DEG2RAD(tab[5]);
                endAngle = DEG2RAD(tab[6]);
            }
            else
            {
                /* Needed in order to set the z coordinate if size == 6 */
                getGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&currentUpperLeftPoint);

                upperLeftPoint[2] = currentUpperLeftPoint[2];
                width = tab[2];
                height = tab[3];
                startAngle = DEG2RAD(tab[4]);
                endAngle = DEG2RAD(tab[5]);
            }

            setGraphicObjectProperty(iObjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

            setGraphicObjectProperty(iObjUID, __GO_WIDTH__, &width, jni_double, 1);
            setGraphicObjectProperty(iObjUID, __GO_HEIGHT__, &height, jni_double, 1);

            setGraphicObjectProperty(iObjUID, __GO_START_ANGLE__, &startAngle, jni_double, 1);
            setGraphicObjectProperty(iObjUID, __GO_END_ANGLE__, &endAngle, jni_double, 1);

            return 0;
        }
        case __GO_TEXT__ :
        {
            int iArentAxes = 0;
            int* piArentAxes = &iArentAxes;
            double position[3];
            int iView = 0;
            int* piView = &iView;

            if ((*numrow **numcol != 2) && (*numrow **numcol != 3))
            {
                Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"), 2, 3, "z");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_PARENT_AXES__, jni_int, (void **)&piArentAxes);
            getGraphicObjectProperty(iArentAxes, __GO_VIEW__, jni_int, (void**)&piView);

            position[0] = tab[0];
            position[1] = tab[1];

            if (iView)
            {
                position[2] = tab[2];
            }
            else
            {
                /*
                 * Required as the position has 3 coordinates, hence the z-coordinate
                 * is set to its current value, which must be retrieved beforehand.
                 * Avoiding doing this would require being able to set only the x and y
                 * coordinates if required.
                 */
                double* currentPosition;
                getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **)&currentPosition);
                position[2] = currentPosition[2];
            }

            setGraphicObjectProperty(iObjUID, __GO_POSITION__, position, jni_double_vector, 3);

            return 0;
        }
        case __GO_SEGS__ :
        {
            int numArrows = 0;
            double* arrowPoints = NULL;

            if ((*numcol != 3) && (*numcol != 2))
            {
                Scierror(999, _("Number of columns must be %d (%d if %s coordinate).\n"), 2, 3, "z");
                return -1;
            }

            if (*numrow % 2 != 0)
            {
                Scierror(999, _("Number of rows must be a multiple of 2.\n"));
                return -1;
            }


            numArrows = *numrow / 2;

            arrowPoints = (double*) MALLOC(3 * numArrows * sizeof(double));

            if (arrowPoints == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
                return -1;
            }

            /*
             * Interlacing ought to be done in the MVC's coordinates
             * set function to avoid the additional code below.
             */
            for (i = 0; i < numArrows; i++)
            {
                arrowPoints[3 * i] = tab[2 * i];
                arrowPoints[3 * i + 1] = tab[2 * numArrows + 2 * i];

                if (*numcol == 3)
                {
                    arrowPoints[3 * i + 2] = tab[4 * numArrows + 2 * i];
                }
                else
                {
                    arrowPoints[3 * i + 2] = 0.0;
                }
            }

            setGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, &numArrows, jni_int, 1);

            setGraphicObjectProperty(iObjUID, __GO_BASE__, arrowPoints, jni_double_vector, 3 * numArrows);

            for (i = 0; i < numArrows; i++)
            {
                arrowPoints[3 * i] = tab[2 * i + 1];
                arrowPoints[3 * i + 1] = tab[2 * numArrows + 2 * i + 1];

                if (*numcol == 3)
                {
                    arrowPoints[3 * i + 2] = tab[4 * numArrows + 2 * i + 1];
                }
                else
                {
                    arrowPoints[3 * i + 2] = 0.0;
                }
            }

            setGraphicObjectProperty(iObjUID, __GO_DIRECTION__, arrowPoints, jni_double_vector, 3 * numArrows);

            FREE(arrowPoints);

            return 0;
        }
        /* DJ.A 2003 */
        /* SCI_SURFACE has been replaced by the MVC's FAC3D and PLOT3D */
        case __GO_FAC3D__ :
        {
            Scierror(999, _("Unhandled data field\n"));
            return -1;
        }
        case __GO_PLOT3D__ :
        {
            Scierror(999, _("Unhandled data field\n"));
            return -1;
        }
        case __GO_MATPLOT__ :
        {
            int nx = 0;
            int ny = 0;
            int gridSize[4];
            int result = 0;

            ny = *numrow;
            nx = *numcol;

            /*
             * The number of points along each dimension is equal to the z data matrix's
             * corresponding dimension plus 1
             */
            gridSize[0] = nx + 1;
            gridSize[1] = 1;
            gridSize[2] = ny + 1;
            gridSize[3] = 1;

            result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

            if (result == FALSE)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
                return -1;
            }

            setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_Z__, tab, jni_double_vector, nx * ny);
            return 0;
        }
        case __GO_FEC__ :
        {
            BOOL result = FALSE;
            int Nnode = 0;
            if (*numcol != 3)
            {
                Scierror(999, _("Number of columns must be %d.\n"), 3);
                return -1;
            }

            Nnode = *numrow;

            /* Resizes the data coordinates array if required */
            result = setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_NUM_VERTICES__, &Nnode, jni_int, 1);

            if (result == FALSE)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
                return -1;
            }

            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_X__, tab, jni_double_vector, Nnode);
            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_Y__, &tab[Nnode], jni_double_vector, Nnode);
            setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_VALUES__, &tab[2 * Nnode], jni_double_vector, Nnode);
            return 0;
        }
        case __GO_FIGURE__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
        case __GO_AXES__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
        case __GO_LEGEND__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
        case __GO_AXIS__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
        case __GO_COMPOUND__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
        /* F.Leray 28.05.04 */
        case __GO_LABEL__ :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
            /*
             * Deactivated for now
             * Same condition as the default one
             */
#if 0
        case SCI_UIMENU:
#endif
        default :
        {
            printSetGetErrorMessage("data");
            return -1;
        }
    }

    return 0;
}

/*-----------------------------------------------------------------------------------*/
/**
 * Check that a color index is within the colormap range or not
 * @param pobjUID object conatining the color
 */
BOOL sciCheckColorIndex(int iObjUID, int colorIndex)
{
    return (colorIndex >= -2) && (colorIndex <= sciGetNumColors(iObjUID) + 2);
}
/*----------------------------------------------------------------------------------*/
