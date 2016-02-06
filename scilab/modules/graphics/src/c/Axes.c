/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
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
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots
 *--------------------------------------------------------------------------*/

#include <string.h>
#include "Axes.h"
#include "DestroyObjects.h"
#include "InitObjects.h"
#include "SetProperty.h"
#include "axesScale.h"
#include "CurrentSubwin.h"

#include "GetProperty.h"
#include "HandleManagement.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "AxesModel.h"
#include "CurrentSubwin.h"

#include "deleteGraphicObject.h"
/*--------------------------------------------------------------------------------*/
static void initSubWinAngles(int iSubWinUID);
static void initSubWinBounds(int iSubWinUID);
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position) */
static void reinitSubWin(int iSubWinUID)
{
    int visible = 0;
    int firstPlot = 0;
    int axisLocation = 0;
    int iLabelUID = 0;
    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    int i = 0;
    int* piChildrenUID = NULL;

    /* Deletes the Axes' children */
    getGraphicObjectProperty(iSubWinUID, __GO_CHILDREN_COUNT__, jni_int, (void **) &piChildrenCount);

    if (iChildrenCount != 0)
    {
        getGraphicObjectProperty(iSubWinUID, __GO_CHILDREN__, jni_int_vector, (void **) &piChildrenUID);

        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            deleteGraphicObject(piChildrenUID[i]);
        }
    }

    initSubWinBounds(iSubWinUID);

    iLabelUID = initLabel(iSubWinUID);
    setGraphicObjectProperty(iSubWinUID, __GO_TITLE__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iSubWinUID);
    setGraphicObjectProperty(iSubWinUID, __GO_X_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iSubWinUID);
    setGraphicObjectProperty(iSubWinUID, __GO_Y_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iSubWinUID);
    setGraphicObjectProperty(iSubWinUID, __GO_Z_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    /* bottom */
    axisLocation = 0;
    setGraphicObjectProperty(iSubWinUID, __GO_X_AXIS_LOCATION__, &axisLocation, jni_int, 1);
    /* left */
    axisLocation = 4;
    setGraphicObjectProperty(iSubWinUID, __GO_Y_AXIS_LOCATION__, &axisLocation, jni_int, 1);

    visible = 1;
    setGraphicObjectProperty(iSubWinUID, __GO_VISIBLE__, &visible, jni_bool, 1);
    firstPlot = 1;
    setGraphicObjectProperty(iSubWinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    initSubWinAngles(iSubWinUID);
}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
static void initSubWinAngles(int iSubWinUID)
{
    int iViewType = 0;
    int* piViewType = &iViewType;
    double* rotationAngles = NULL;
    int iAxesModelUID = getAxesModel();

    getGraphicObjectProperty(iAxesModelUID, __GO_VIEW__, jni_int, (void **) &piViewType);
    setGraphicObjectProperty(iSubWinUID, __GO_VIEW__, &iViewType, jni_int, 1);

    getGraphicObjectProperty(iAxesModelUID, __GO_ROTATION_ANGLES__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(iSubWinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    getGraphicObjectProperty(iAxesModelUID, __GO_ROTATION_ANGLES_3D__, jni_double_vector, (void **) &rotationAngles);
    setGraphicObjectProperty(iSubWinUID, __GO_ROTATION_ANGLES_3D__, rotationAngles, jni_double_vector, 2);
}

/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
static void initSubWinBounds(int iSubWinUID)
{
    double* dataBounds = NULL;
    double* realDataBounds = NULL;
    int iAxesModelUID = getAxesModel();
    int linLogFlag = 0;

    setGraphicObjectProperty(iSubWinUID, __GO_X_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);
    setGraphicObjectProperty(iSubWinUID, __GO_Y_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);
    setGraphicObjectProperty(iSubWinUID, __GO_Z_AXIS_LOG_FLAG__, &linLogFlag, jni_bool, 1);

    getGraphicObjectProperty(iAxesModelUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);
    setGraphicObjectProperty(iSubWinUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    getGraphicObjectProperty(iAxesModelUID, __GO_REAL_DATA_BOUNDS__, jni_double_vector, (void **)&realDataBounds);
    setGraphicObjectProperty(iSubWinUID, __GO_REAL_DATA_BOUNDS__, realDataBounds, jni_double_vector, 6);
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing(void)
{
    int iAutoClear = 0;
    int* piAutoClear = &iAutoClear;

    //  nbCheckRedraw++;
    //  fprintf(stderr, "[DEBUG] checkRedrawing : %d\n", nbCheckRedraw);
    int iSubWinID = getCurrentSubWin();

    getGraphicObjectProperty(iSubWinID, __GO_AUTO_CLEAR__, jni_bool, (void **)&piAutoClear);

    if (iAutoClear)
    {
        reinitSubWin(iSubWinID);
        return TRUE;
    }

    return FALSE;
}
/*--------------------------------------------------------------------------------*/
sciLegendPlace propertyNameToLegendPlace(const char * string)
{
    if (strcmp(string, "in_upper_right") == 0)
    {
        return SCI_LEGEND_IN_UPPER_RIGHT;
    }
    else if (strcmp(string, "in_upper_left") == 0)
    {
        return SCI_LEGEND_IN_UPPER_LEFT;
    }
    else if (strcmp(string, "in_lower_right") == 0)
    {
        return SCI_LEGEND_IN_LOWER_RIGHT;
    }
    else if (strcmp(string, "in_lower_left") == 0)
    {
        return SCI_LEGEND_IN_LOWER_LEFT;
    }
    else if (strcmp(string, "out_upper_right") == 0)
    {
        return SCI_LEGEND_OUT_UPPER_RIGHT;
    }
    else if (strcmp(string, "out_upper_left") == 0)
    {
        return SCI_LEGEND_OUT_UPPER_LEFT;
    }
    else if (strcmp(string, "out_lower_right") == 0)
    {
        return SCI_LEGEND_OUT_LOWER_RIGHT;
    }
    else if (strcmp(string, "out_lower_left") == 0)
    {
        return SCI_LEGEND_OUT_LOWER_LEFT;
    }
    else if (strcmp(string, "upper_caption") == 0)
    {
        return SCI_LEGEND_UPPER_CAPTION;
    }
    else if (strcmp(string, "lower_caption") == 0)
    {
        return SCI_LEGEND_LOWER_CAPTION;
    }
    else if (strcmp(string, "by_coordinates") == 0)
    {
        return SCI_LEGEND_BY_COORDINATES;
    }
    else
    {
        return SCI_LEGEND_POSITION_UNSPECIFIED;
    }
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a boolean log flag to the character format
 */
char getTextLogFlag(int logFlag)
{
    if (logFlag)
    {
        return 'l';
    }
    else
    {
        return 'n';
    }
}
/*--------------------------------------------------------------------------------*/
/*
 * Converts a character log flag to the equivalent boolean
 */
int getBooleanLogFlag(char logFlag)
{
    if (logFlag == 'l')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*--------------------------------------------------------------------------------*/

