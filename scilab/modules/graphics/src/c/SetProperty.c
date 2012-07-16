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
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 *      and -1 if an error occured.
 --------------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

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

#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FigureModel.h"
#include "AxesModel.h"

/**sciSetLineWidth
 * Sets the line width
 */
int sciSetLineWidth (char * pobjUID, double linewidth)
{
    BOOL status = FALSE;

    if (linewidth < 0)
    {
        Scierror(999, _("Line width must be greater than %d.\n"), 0);
        return -1;
    }
    else
    {
        status = setGraphicObjectProperty(pobjUID, __GO_LINE_THICKNESS__, &linewidth, jni_double, 1);

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
int sciSetLineStyle(char * pobjUID, int linestyle)
{
    BOOL status = FALSE;
    if (linestyle < 0)
    {
        Scierror(999, _("The line style must be greater than %d.\n"), 0);
        return -1;
    }
    else
    {
        status = setGraphicObjectProperty(pobjUID, __GO_LINE_STYLE__, &linestyle, jni_int, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }

    printSetGetErrorMessage("line_style");
    return -1;
}

int sciSetMarkSize( char * pobjUID, int marksize )
{
    if (marksize < 0)
    {
        Scierror(999, _("The mark size must be greater or equal than %d.\n"), 0);
        return -1;
    }
    else
    {
        BOOL status = setGraphicObjectProperty(pobjUID, __GO_MARK_SIZE__, &marksize, jni_int, 1);

        if (status == TRUE)
        {
            return 0;
        }
    }

    printSetGetErrorMessage("mark_size");
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
int sciSetText (char * pobjUID, char ** text, int nbRow, int nbCol)
{
    int dimensions[2];
    BOOL status = FALSE;

    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(text, nbRow, nbCol);

    dimensions[0] = nbRow;
    dimensions[1] = nbCol;

    status = setGraphicObjectProperty(pobjUID, __GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    if (status != TRUE)
    {
        printSetGetErrorMessage("text");
        return -1;
    }

    status = setGraphicObjectProperty(pobjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, dimensions[0] * dimensions[1]);

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

/**sciSetSelectedSubWin
 * Determines wich SubWin is selected or not. WARNING TO BE DEFINED.
 * It has been adapted to the MVC. Its should be implemented entirely
 * within the MVC (as the setGraphicObjectRelationship function).
 * @param char * psubwinobj: the pointer to the entity sub window
 * @return 0 if OK or -1 if NOT OK
 */
int
sciSetSelectedSubWin (char * psubwinobjUID)
{
    char* type = NULL;
    char* parent = NULL;

    getGraphicObjectProperty(psubwinobjUID, __GO_TYPE__, jni_string, (void **)&type);

    /* Check that the object is an AXES */
    if (strcmp(type, __GO_AXES__) != 0)
    {
        Scierror(999, _("Handle is not a SubWindow.\n"));
        return -1;
    }

    getGraphicObjectProperty(psubwinobjUID, __GO_PARENT__, jni_string, (void **)&parent);

    setGraphicObjectProperty(parent, __GO_SELECTED_CHILD__, psubwinobjUID, jni_string, 1);

    return 0;
}

/*-------------------------------------------------------------------------------*/

/**sciSetPoint
 * sets points of the entity, and a pointer to the number of points
 */
/** MAJ pour le 3D DJ.Abdemouche 2003**/
int
sciSetPoint(char * pthis, double *tab, int *numrow, int *numcol)
{
    char* type = NULL;
    int i = 0, n1 = 0;

    getGraphicObjectProperty(pthis, __GO_TYPE__, jni_string, (void **)&type);

    /*
     * switch over sciGetEntityType replaced by object type string comparisons
     * Still required as we have no better way to do this for the moment
     */
    if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        BOOL result;
        int numElementsArray[2];
        int zCoordinatesSet;

        if ( (*numcol != 3) && (*numcol != 2) && (*numcol != 0) )
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
        result = setGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

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

        if ( *numcol > 0 )
        {
            setGraphicObjectProperty(pthis, __GO_DATA_MODEL_X__, tab, jni_double_vector, n1);
            setGraphicObjectProperty(pthis, __GO_DATA_MODEL_Y__, &tab[n1], jni_double_vector, n1);

            if (*numcol == 3)
            {
                setGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z__, &tab[2 * n1], jni_double_vector, n1);
                zCoordinatesSet = 1;
            }
            else
            {
                zCoordinatesSet = 0;
            }

            /* Required for now to indicate that the z coordinates have been set or not */
            setGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_int, 1);
        }

        return 0;
    }
    else if (strcmp(type, __GO_RECTANGLE__) == 0)
    {
        double* currentUpperLeftPoint = NULL;
        double upperLeftPoint[3];
        int widthIndex = 2;
        int size = *numrow * *numcol;

        if ( size != 5 && size != 4 )
        {
            Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"), 4, 5, "z");
            return -1;
        }

        upperLeftPoint[0] = tab[0];
        upperLeftPoint[1] = tab[1];

        if ( size == 5 )
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
            getGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&currentUpperLeftPoint);
            upperLeftPoint[2] = currentUpperLeftPoint[2];
        }

        setGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

        /* check that the height and width are positive */
        if ( tab[widthIndex] < 0.0 || tab[widthIndex + 1] < 0.0 )
        {
            Scierror(999, "Width and height must be positive.\n") ;
            return -1 ;
        }

        setGraphicObjectProperty(pthis, __GO_WIDTH__, &tab[widthIndex], jni_double, 1);
        setGraphicObjectProperty(pthis, __GO_HEIGHT__, &tab[widthIndex + 1], jni_double, 1);

        return 0;
    }
    else if (strcmp(type, __GO_ARC__) == 0)
    {
        double startAngle = 0.;
        double endAngle = 0.;
        double upperLeftPoint[3];
        double width = 0.;
        double height = 0.;
        double* currentUpperLeftPoint = NULL;
        int size = 0;

        size = *numrow * *numcol;

        if ((size != 7) && (size != 6))
        {
            Scierror(999, _("Number of elements must be %d (%d if z coordinate )\n"), 6, 7);
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
            getGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&currentUpperLeftPoint);

            upperLeftPoint[2] = currentUpperLeftPoint[2];
            width = tab[2];
            height = tab[3];
            startAngle = DEG2RAD(tab[4]);
            endAngle = DEG2RAD(tab[5]);
        }

        setGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

        setGraphicObjectProperty(pthis, __GO_WIDTH__, &width, jni_double, 1);
        setGraphicObjectProperty(pthis, __GO_HEIGHT__, &height, jni_double, 1);

        setGraphicObjectProperty(pthis, __GO_START_ANGLE__, &startAngle, jni_double, 1);
        setGraphicObjectProperty(pthis, __GO_END_ANGLE__, &endAngle, jni_double, 1);

        return 0;
    }
    else if (strcmp(type, __GO_TEXT__) == 0)
    {
        char* parentAxes = NULL;
        double position[3];
        int iView = 0;
        int* piView = &iView;

        if ((*numrow * *numcol != 2) && (*numrow * *numcol != 3))
        {
            Scierror(999, _("Number of elements must be %d (%d if %s coordinate).\n"), 2, 3, "z");
            return -1;
        }

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);
        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

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
            getGraphicObjectProperty(pthis, __GO_POSITION__, jni_double_vector, (void **)&currentPosition);
            position[2] = currentPosition[2];
        }

        setGraphicObjectProperty(pthis, __GO_POSITION__, position, jni_double_vector, 3);

        return 0;
    }
    else if (strcmp(type, __GO_SEGS__) == 0)
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

        setGraphicObjectProperty(pthis, __GO_NUMBER_ARROWS__, &numArrows, jni_int, 1);

        setGraphicObjectProperty(pthis, __GO_BASE__, arrowPoints, jni_double_vector, 3 * numArrows);

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

        setGraphicObjectProperty(pthis, __GO_DIRECTION__, arrowPoints, jni_double_vector, 3 * numArrows);

        FREE(arrowPoints);

        return 0;
    }
    /* DJ.A 2003 */
    /* SCI_SURFACE has been replaced by the MVC's FAC3D and PLOT3D */
    else if (strcmp(type, __GO_FAC3D__) == 0)
    {
        Scierror(999, _("Unhandled data field\n"));
        return -1;
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        Scierror(999, _("Unhandled data field\n"));
        return -1;
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
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

        result = setGraphicObjectProperty(pthis, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

        if (result == FALSE)
        {
            Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
            return -1;
        }

        setGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z__, tab, jni_double_vector, nx * ny);
    }
    else if (strcmp(type, __GO_FEC__) == 0)
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
        result = setGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_VERTICES__, &Nnode, jni_int, 1);

        if (result == FALSE)
        {
            Scierror(999, _("%s: No more memory.\n"), "sciSetPoint");
            return -1;
        }

        setGraphicObjectProperty(pthis, __GO_DATA_MODEL_X__, tab, jni_double_vector, Nnode);
        setGraphicObjectProperty(pthis, __GO_DATA_MODEL_Y__, &tab[Nnode], jni_double_vector, Nnode);
        setGraphicObjectProperty(pthis, __GO_DATA_MODEL_VALUES__, &tab[2 * Nnode], jni_double_vector, Nnode);
    }
    else if (strcmp(type, __GO_FIGURE__) == 0)
    {
        printSetGetErrorMessage("data");
        return -1;
    }
    else if (strcmp(type, __GO_AXES__) == 0)
    {
        printSetGetErrorMessage("data");
        return -1;
    }
    else if (strcmp(type, __GO_LEGEND__) == 0)
    {
        printSetGetErrorMessage("data");
        return -1;
    }
    else if (strcmp(type, __GO_AXIS__) == 0)
    {
        printSetGetErrorMessage("data");
        return -1;
    }
    else if (strcmp(type, __GO_COMPOUND__) == 0)
    {
        printSetGetErrorMessage("data");
        return -1;
    }
    /* F.Leray 28.05.04 */
    else if (strcmp(type, __GO_LABEL__) == 0)
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
    else
    {
        printSetGetErrorMessage("data");
        return -1;
    }

    return 0;
}

/*----------------------------------------------------------------------------------*/
/**
 * In new graphic style, select a window and create one if not already done.
 */
int sciSetUsedWindow( int winNum )
{
    // FIXME
    abort();
#if 0
    return 1;
    /* select or create the window in the driver */
    if ( sciHasFigures() && sciGetNum( sciGetCurrentFigure() ) == winNum )
    {
        /* nothing to do */
        return 1 ;
    }

    return sciInitUsedWindow( winNum ) ;
#endif
    return 1;
}

int sciInitBoxType( char * pobjUID, EAxesBoxType type )
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->axes.rect = type ;
            /* redondant information in scilab :'( */
            switch ( type )
            {
                case BT_OFF:
                    pSUBWIN_FEATURE(pobjUID)->axes.flag[2] = 0 ;
                    break ;
                case BT_ON:
                    pSUBWIN_FEATURE(pobjUID)->axes.flag[2] = 4 ;
                    break ;
                case BT_HIDDEN_AXES:
                    pSUBWIN_FEATURE(pobjUID)->axes.flag[2] = 2 ;
                    break ;
                case BT_BACK_HALF:
                    pSUBWIN_FEATURE(pobjUID)->axes.flag[2] = 3 ;
                    break ;
            }
            return 0;
            break;
        default:
            printSetGetErrorMessage("box_type");
            return -1 ;
            break;
    }
#endif
    return -1 ;
}

/* set the property of the axes box  */
int sciSetBoxType( char * pobjUID, EAxesBoxType type )
{

    if ( sciGetBoxType((sciPointObj *) pobjUID ) == type )
    {
        return 1 ;
    }
    return sciInitBoxType( pobjUID, type ) ;

}


int sciInitIsBoxed( char * pobjUID, BOOL isboxed )
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->isboxed = isboxed;
            return 0;
            break;
        case SCI_SUBWIN:
            Scierror(999, _("%s: please use %s instead.\n"), "sciSetIsBoxed", "sciSetBoxType");
            if ( isboxed )
            {
                pSUBWIN_FEATURE(pobjUID)->axes.rect = BT_ON ;
            }
            else
            {
                pSUBWIN_FEATURE(pobjUID)->axes.rect = BT_OFF ;
            }
            return 0;
            break;
        case SCI_LABEL:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_FIGURE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_LEGEND:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            printSetGetErrorMessage("box_type");
            return -1;
            break;
    }
#endif
    return -1 ;
}

/**sciSetIsBoxed
 * Sets the box existence
 */
int
sciSetIsBoxed (char * pobjUID, BOOL isboxed)
{

    if ( sciGetIsBoxed((sciPointObj *) pobjUID ) == isboxed )
    {
        return 1 ;
    }
    return sciInitIsBoxed( pobjUID, isboxed ) ;

}


int
sciInitNbXSubTics(char * pobjUID, int nbsubtics)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->axes.nbsubtics[0] = nbsubtics;
            break;
        default:
            printSetGetErrorMessage("subwin_type required");
            return -1 ;
            break;
    }
#endif
    return 0 ;
}

/**sciSetNbXSubTics
 * Sets the number of x subticks
 */
int
sciSetNbXSubTics(char * pobjUID, int nbsubtics)
{
    int curr_nbsubtics[3];

    sciGetNbSubTics((sciPointObj*) pobjUID,  curr_nbsubtics);

    if ( curr_nbsubtics[0] == nbsubtics )
    {
        return 1 ;
    }
    return sciInitNbXSubTics( pobjUID, nbsubtics ) ;
}

int
sciInitNbYSubTics(char * pobjUID, int nbsubtics)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->axes.nbsubtics[1] = nbsubtics;
            break;
        default:
            printSetGetErrorMessage("subwin_type required");
            return -1 ;
            break;
    }
#endif
    return 0 ;
}

/**sciSetNbYSubTics
 * Sets the number of y subticks
 */
int
sciSetNbYSubTics(char * pobjUID, int nbsubtics)
{
    int curr_nbsubtics[3];

    sciGetNbSubTics((sciPointObj*) pobjUID,  curr_nbsubtics);

    if ( curr_nbsubtics[1] == nbsubtics )
    {
        return 1 ;
    }
    return sciInitNbYSubTics( pobjUID, nbsubtics ) ;
}

int
sciInitNbZSubTics(char * pobjUID, int nbsubtics)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->axes.nbsubtics[2] = nbsubtics;
            break;
        default:
            printSetGetErrorMessage("subwin_type required");
            return -1 ;
            break;
    }
#endif
    return 0 ;
}

/**sciSetNbZSubTics
 * Sets the number of x subticks
 */
int
sciSetNbZSubTics(char * pobjUID, int nbsubtics)
{
    int curr_nbsubtics[3];

    sciGetNbSubTics((sciPointObj*) pobjUID,  curr_nbsubtics);

    if ( curr_nbsubtics[2] == nbsubtics )
    {
        return 1 ;
    }
    return sciInitNbZSubTics( pobjUID, nbsubtics ) ;
}


int
sciSetInterpVector(char * pobjUID, int size, int * value)
{
    // FIXME
    abort();
#if 0
    int i;

    FREE(pPOLYLINE_FEATURE(pobjUID)->scvector);

    if ((pPOLYLINE_FEATURE(pobjUID)->scvector = MALLOC(size * sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "sciSetInterpVector") ;
        return -1;
    }

    for (i = 0; i < size; i++)
        pPOLYLINE_FEATURE(pobjUID)->scvector[i] = value[i];
#endif
    return 0;
}

int sciInitAutoRotation( char * pobjUID, BOOL value )
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_LABEL:
            pLABEL_FEATURE(pobjUID)->auto_rotation = value ;
            return 0;
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_TEXT:
        case SCI_FIGURE:
        case SCI_SUBWIN:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FEC:
        case SCI_SEGS:
        case SCI_LEGEND:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            printSetGetErrorMessage("auto_rotation");
            return -1 ;
            break;
    }
#endif
    return -1;

}

/* set the auto_rotation property of an object */
int sciSetAutoRotation ( char * pobjUID, BOOL value )
{

    if ( sciGetAutoRotation((sciPointObj*) pobjUID ) == value )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitAutoRotation( pobjUID, value ) ;

}


int sciInitAutoPosition( char * pobjUID, BOOL value )
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_LABEL:
            pLABEL_FEATURE(pobjUID)->auto_position = value ;
            return 0;
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_TEXT:
        case SCI_FIGURE:
        case SCI_SUBWIN:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FEC:
        case SCI_SEGS:
        case SCI_LEGEND:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            printSetGetErrorMessage("auto_position");
            return -1 ;
            break;
    }
#endif
    return -1 ;
}

/* set the auto_position property of an object */
int sciSetAutoPosition ( char * pobjUID, BOOL value )
{

    if ( sciGetAutoPosition((sciPointObj*) pobjUID ) == value )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitAutoPosition( pobjUID, value ) ;

}

/*---------------------------------------------------------------------------*/
int checkPercent( char * string )
{
    /* check for the percent in the string */
    char * firstPercent  = strchr( string, '%' ) ;

    if ( firstPercent == NULL )
    {
        /* no % character found */
        return 0 ;
    }
    else if ( firstPercent[1] != 'd' )
    {
        /* a %something with something != d has been found */
        return -1 ;
    }
    else
    {
        /* here we have found a first %d, check if there is not any more % */
        firstPercent++ ;
        firstPercent = strchr( firstPercent, '%' ) ;
        if ( firstPercent == NULL )
        {
            /* no other % character found */
            return 1 ;
        }
        else
        {
            return -1 ;
        }
    }
}
/*---------------------------------------------------------------------------*/
int sciInitAutoSize( char * pobjUID, BOOL autoSize )
{
    // FIXME
#if 0
    switch ( sciGetEntityType (pobjUID) )
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->autoSize = autoSize ;
            return 0 ;
        default:
            printSetGetErrorMessage("text_box_mode");
            return -1 ;
            break;
    }
#endif
    return -1;

}
/*---------------------------------------------------------------------------*/
int sciSetAutoSize( char * pobjUID, BOOL autoSize )
{
    if ( sciGetAutoSize((sciPointObj*) pobjUID ) == autoSize )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitAutoSize( pobjUID, autoSize ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitAlignment( char * pobjUID, sciTextAlignment align )
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType (pobjUID))
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->stringsAlign = align ;
            return 0 ;
        default:
            printSetGetErrorMessage("alignment");
            return -1 ;
    }
#endif
    return -1 ;
}
/*-----------------------------------------------------------------------------------*/
int sciSetAlignment( char * pobjUID, sciTextAlignment align )
{
    if ( sciGetAlignment((sciPointObj*) pobjUID ) == align )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitAlignment( pobjUID, align ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitUserSize( char * pobjUID, double width, double height )
{
    // FIXME
    abort();
#if 0
    switch ( sciGetEntityType( pobjUID ) )
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->userSize[0] = width ;
            pTEXT_FEATURE(pobjUID)->userSize[1] = height ;
            return 0 ;
        default:
            printSetGetErrorMessage("text_box");
            return -1 ;
    }
#endif
    return -1;
}
/*-----------------------------------------------------------------------------------*/
int sciSetUserSize( char * pobjUID, double width, double height )
{
    double curWidth  = 0.;
    double curHeight = 0.;
    sciGetUserSize((sciPointObj*) pobjUID, &curWidth, &curHeight ) ;
    if ( curWidth == width && curHeight == height )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitUserSize( pobjUID, width, height ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitCenterPos( char * pobjUID, BOOL newCP )
{
    // FIXME
    abort();
#if 0
    switch ( sciGetEntityType( pobjUID ) )
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->centeredPos = newCP ;
            return 0 ;
        default:
            printSetGetErrorMessage("data");
            return -1 ;
    }
#endif
    return -1;

}
/*-----------------------------------------------------------------------------------*/
int sciSetCenterPos( char * pobjUID, BOOL newCP )
{
    if ( sciGetCenterPos((sciPointObj*) pobjUID ) == newCP )
    {
        /* nothing to do */
        return 1 ;
    }
    return sciInitCenterPos( pobjUID, newCP ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitEventHandler( char * pobjUID, char * name )
{
    // FIXME
    abort();
    return -1;
}
/*-----------------------------------------------------------------------------------*/
int sciSetEventHandler( char * pobjUID, char * name )
{
    if ( strcmp( sciGetEventHandler((sciPointObj*)pobjUID), name ) == 0 )
    {
        /* nothing to do */
        return 1 ;
    }

    return sciInitEventHandler( pobjUID, name ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciInitIsEventHandlerEnable( char * pobjUID, BOOL enable )
{
    // FIXME
    abort();
    return -1;
}
/*-----------------------------------------------------------------------------------*/
int sciSetIsEventHandlerEnable( char * pobjUID, BOOL enable )
{
    if ( sciGetIsEventHandlerEnable((sciPointObj*)pobjUID) == enable )
    {
        /* nothing to do */
        return 1 ;
    }

    return sciInitIsEventHandlerEnable( pobjUID, enable ) ;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set data-bounds defined by the user.
 * @param bounds [Xmin,Xmax,Ymain,Ymax,Zmin,Zmax] vector.
 */
int sciSetDataBounds( char * pobjUID, double bounds[6] )
{
    // FIXME
    abort();
#if 0
    int i;
    switch ( sciGetEntityType(pobjUID) )
    {
        case SCI_SUBWIN:
            for ( i = 0 ; i < 6 ; i++ )
            {
                pSUBWIN_FEATURE(pobjUID)->SRect[i] = bounds[i] ;
            }
            return 0;
        case SCI_SURFACE:
            for ( i = 0 ; i < 6 ; i++ )
            {
                pSURFACE_FEATURE(pobjUID)->ebox[i] = bounds[i] ;
            }
            return 0;
        default:
            printSetGetErrorMessage("data_bounds");
            return -1 ;

    }
#endif
    return -1;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Set the displayed data bounds of a subwin object.
 */
int sciSetRealDataBounds(char * pobjUID, const double bounds[6])
{
    // FIXME
    abort();
#if 0
    int i;
    switch ( sciGetEntityType(pobjUID) )
    {
        case SCI_SUBWIN:
            for ( i = 0 ; i < 6 ; i++ )
            {
                pSUBWIN_FEATURE(pobjUID)->FRect[i] = bounds[i] ;
            }
            return 0;
        default:
            printSetGetErrorMessage("data_bounds");
            return -1 ;

    }
#endif
    return -1;
}
/*-----------------------------------------------------------------------------------*/
/**
 * To set the pixmap mode of a figure to on or off
 */
int sciInitPixmapMode(char * pobjUID, BOOL onOrOff)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType(pobjUID))
    {
        case SCI_FIGURE:
            pFIGURE_FEATURE(pobjUID)->pixmapMode = onOrOff;
            return 0;
        default:
            printSetGetErrorMessage("pixmap");
            return -1;
    }
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
/**
 * To set the pixmap mode of a figure to on or off
 */
int sciSetPixmapMode(char * pobjUID, BOOL onOrOff)
{
    if (sciGetPixmapMode((sciPointObj*)pobjUID) == onOrOff)
    {
        /* Nothing to do */
        return 1;
    }
    return sciInitPixmapMode(pobjUID, onOrOff);
}
/*----------------------------------------------------------------------------------*/
int sciInitTextPos( char * pobjUID, double posX, double posY, double posZ)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType(pobjUID))
    {
        case SCI_TEXT:
            pTEXT_FEATURE(pobjUID)->x = posX;
            pTEXT_FEATURE(pobjUID)->y = posY;
            pTEXT_FEATURE(pobjUID)->z = posZ;
            return 0;
        case SCI_LABEL:
            return sciInitTextPos(pLABEL_FEATURE(pobjUID)->text, posX, posY, posZ);
        default:
            printSetGetErrorMessage("data");
            return -1;
    }
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
/**
 * Set the position of a label or text object.
 */
int sciSetTextPos( char * pobjUID, double posX, double posY, double posZ)
{
    double curPos[3];
    sciGetTextPos((sciPointObj*)pobjUID, curPos);
    if ( curPos[0] == posX && curPos[1] == posY && curPos[2] == posZ )
    {
        /* nothing to do */
        return 1;
    }
    return sciInitTextPos(pobjUID, posX, posY, posZ);
}
/*----------------------------------------------------------------------------------*/
int sciInitAutoTicks(char * pobjUID, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType(pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->axes.auto_ticks[0] = autoTicksX;
            pSUBWIN_FEATURE(pobjUID)->axes.auto_ticks[1] = autoTicksY;
            pSUBWIN_FEATURE(pobjUID)->axes.auto_ticks[2] = autoTicksZ;
            return 0;
        default:
            printSetGetErrorMessage("auto_ticks");
            return -1;
    }
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
int sciSetAutoTicks(char * pobjUID, BOOL autoTicksX, BOOL autoTicksY, BOOL autoTicksZ)
{
    BOOL curAutoTicks[3];
    sciGetAutoTicks((sciPointObj*)pobjUID, curAutoTicks);

    if (curAutoTicks[0] == autoTicksX && curAutoTicks[1] == autoTicksY && curAutoTicks[2] == autoTicksZ)
    {
        // nothing to do
        return 1;
    }

    return sciInitAutoTicks(pobjUID, autoTicksX, autoTicksY, autoTicksZ);

}
/*----------------------------------------------------------------------------------*/
/**
 * Specify a new zoom box for a subwin object.
 * @param zoomBox [xMin, xMax, yMin, yMax, zMin, zMax] vector.
 */
int sciSetZoomBox(char * pobjUID, const double zoomBox[6])
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType(pobjUID))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pobjUID)->ZRect[0] = zoomBox[0];
            pSUBWIN_FEATURE(pobjUID)->ZRect[1] = zoomBox[1];
            pSUBWIN_FEATURE(pobjUID)->ZRect[2] = zoomBox[2];
            pSUBWIN_FEATURE(pobjUID)->ZRect[3] = zoomBox[3];
            pSUBWIN_FEATURE(pobjUID)->ZRect[4] = zoomBox[4];
            pSUBWIN_FEATURE(pobjUID)->ZRect[5] = zoomBox[5];
            return 0;
        default:
            printSetGetErrorMessage("zoom_box");
            return -1;
    }
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
int sciInitImmediateDrawingMode(char * pobjUID, BOOL autoRedraw)
{
    // FIXME
    abort();
#if 0
    switch (sciGetEntityType(pobjUID))
    {
        case SCI_FIGURE:
            pFIGURE_FEATURE(pobjUID)->auto_redraw = autoRedraw;
            return 0;
        default:
            printSetGetErrorMessage("immediate_drawing");
            return -1;
    }
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
int sciSetImmediateDrawingMode(char * pobjUID, BOOL autoRedraw)
{
    // FIXME
    abort();
#if 0
    if (sciGetImmediateDrawingMode(pobjUID) == autoRedraw)
    {
        // nothing to do
        return 1;
    }
    return sciInitImmediateDrawingMode(pobjUID, autoRedraw);
#endif
    return -1;
}
/*----------------------------------------------------------------------------------*/
/**
 * Check that a color index is within the colormap range or not
 * @param pobjUID object conatining the color
 */
BOOL sciCheckColorIndex(char * pobjUID, int colorIndex)
{
    return (colorIndex >= -2) && (colorIndex <= sciGetNumColors(pobjUID) + 2);
}
/*----------------------------------------------------------------------------------*/
