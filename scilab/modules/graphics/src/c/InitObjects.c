/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
 *    This file contains all functions used to Init or Re-Init the window
 *    (Figure and/or Subwin) to the default graphics properties.
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "os_string.h"

#include "InitObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DestroyObjects.h"
#include "CloneObjects.h"
#include "BuildObjects.h"
#include "sciprint.h"
#include "HandleManagement.h"
#include "BasicAlgos.h"

#include "sci_malloc.h"             /* MALLOC */
#include "localization.h"

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

#include "AxesModel.h"
#include "FigureModel.h"

#define LABEL_BUFFER_LENGTH	128

static char error_message[70];  /* DJ.A 08/01/04 */

unsigned short defcolors[] =
{
    0, 0, 0,                    /* Black: DEFAULTBLACK */
    0, 0, 255,                  /* Blue */
    0, 255, 0,                  /* Green */
    0, 255, 255,                /* Cyan */
    255, 0, 0,                  /* Red */
    255, 0, 255,                /* Magenta */
    255, 255, 0,                /* Yellow */
    255, 255, 255,              /* White: DEFAULTWHITE */
    0, 0, 144,                  /* Blue4 */
    0, 0, 176,                  /* Blue3 */
    0, 0, 208,                  /* Blue2 */
    135, 206, 255,              /* LtBlue */
    0, 144, 0,                  /* Green4 */
    0, 176, 0,                  /* Green3 */
    0, 208, 0,                  /* Green2 */
    0, 144, 144,                /* Cyan4 */
    0, 176, 176,                /* Cyan3 */
    0, 208, 208,                /* Cyan2 */
    144, 0, 0,                  /* Red4 */
    176, 0, 0,                  /* Red3 */
    208, 0, 0,                  /* Red2 */
    144, 0, 144,                /* Magenta4 */
    176, 0, 176,                /* Magenta3 */
    208, 0, 208,                /* Magenta2 */
    128, 48, 0,                 /* Brown4 */
    160, 64, 0,                 /* Brown3 */
    192, 96, 0,                 /* Brown2 */
    255, 128, 128,              /* Pink4 */
    255, 160, 160,              /* Pink3 */
    255, 192, 192,              /* Pink2 */
    255, 224, 224,              /* Pink */
    255, 215, 0                 /* Gold */
};

void InitFigureModel(int iFiguremdlUID)
{
    int iZero = 0;
    BOOL bTrue = TRUE;
    BOOL bFalse = FALSE;

    int defaultBackground = -2;
    int m = NUMCOLORS_SCI;
    int i = 0;
    double *pdblColorMap = (double*)MALLOC(m * 3 * sizeof(double));

    int piFigurePosition[2] = { 200, 200 };
    int piFigureSize[2] = { 620, 590 };
    int piAxesSize[2] = { 610, 460 };

    // FIXME : inline this function here ...

    sciInitGraphicMode(iFiguremdlUID);
    // Name
    setGraphicObjectProperty(iFiguremdlUID, __GO_NAME__, _("Graphic window number %d"), jni_string, 1);

    // Id
    setGraphicObjectProperty(iFiguremdlUID, __GO_ID__, &iZero, jni_int, 1);
    // pModelData
    // isselected ?? (No more used)
    // rotstyle = unary (0)
    setGraphicObjectProperty(iFiguremdlUID, __GO_ROTATION_TYPE__, &iZero, jni_int, 1);

    // visible
    setGraphicObjectProperty(iFiguremdlUID, __GO_VISIBLE__, &bTrue, jni_bool, 1);
    // immediateDrawingMode
    setGraphicObjectProperty(iFiguremdlUID, __GO_IMMEDIATE_DRAWING__, &bTrue, jni_bool, 1);
    // background
    setGraphicObjectProperty(iFiguremdlUID, __GO_BACKGROUND__, &defaultBackground, jni_int, 1);
    // position
    setGraphicObjectProperty(iFiguremdlUID, __GO_POSITION__, piFigurePosition, jni_int_vector, 2);
    // size
    setGraphicObjectProperty(iFiguremdlUID, __GO_SIZE__, piFigureSize, jni_int_vector, 2);

    // auto_resize
    setGraphicObjectProperty(iFiguremdlUID, __GO_AUTORESIZE__, &bTrue, jni_bool, 1);
    // axes_size
    setGraphicObjectProperty(iFiguremdlUID, __GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);

    /*
     * user data
     * NULL has been replaced by the empty string as the third argument in order to
     * avoid a crash due to giws 1.2.4
     */
    //setGraphicObjectProperty(iFiguremdlUID, __GO_USER_DATA__, "", jni_string, 0);
    // Size of user data
    //setGraphicObjectProperty(iFiguremdlUID, __GO_USER_DATA_SIZE__, &iZero, jni_int, 1);
    // Info Message
    setGraphicObjectProperty(iFiguremdlUID, __GO_INFO_MESSAGE__, "", jni_string, 1);
    // Event Handler
    setGraphicObjectProperty(iFiguremdlUID, __GO_EVENTHANDLER__, "", jni_string, 1);
    // Event Handler Enable
    setGraphicObjectProperty(iFiguremdlUID, __GO_EVENTHANDLER_ENABLE__, &bFalse, jni_bool, 1);
    // Tag
    setGraphicObjectProperty(iFiguremdlUID, __GO_TAG__, "", jni_string, 1);

    if (pdblColorMap == NULL)
    {
        sprintf(error_message, _("%s: No more memory.\n"), "InitFigureModel");
        return;
    }

    for (i = 0; i < m; i++)
    {
        pdblColorMap[i] = (double)(defcolors[3 * i] / 255.0);
        pdblColorMap[i + m] = (double)(defcolors[3 * i + 1] / 255.0);
        pdblColorMap[i + 2 * m] = (double)(defcolors[3 * i + 2] / 255.0);
    }
    // ColorMap
    setGraphicObjectProperty(iFiguremdlUID, __GO_COLORMAP__, pdblColorMap, jni_double_vector, 3 * m);
    FREE(pdblColorMap);

    // Parent
    {
        int parent = 0;
        setGraphicObjectProperty(iFiguremdlUID, __GO_PARENT__, &parent, jni_int, 1);
    }

    // Default menus
    buildFigureMenuBar(iFiguremdlUID);
}

/* DJ.A 08/01/04 */
int C2F(graphicsmodels) (void)
{
    double margins[4];
    double clipRegion[4];
    int clipRegionSet = 0;
    int firstPlot = 0;
    int result = 0;

    int iFiguremdlUID = 0;
    int iAxesmdlUID = 0;

    /*
     ** Init Figure Model
     */

    // Create default figure by Asking MVC a new one.
    iFiguremdlUID = createGraphicObject(__GO_FIGUREMODEL__);
    setFigureModel(iFiguremdlUID);
    InitFigureModel(iFiguremdlUID);

    sciInitGraphicMode(iFiguremdlUID);

    /*
     ** Init Axes Model
     */

    // Create default Axes by Asking MVC a new one.
    iAxesmdlUID = createGraphicObject(__GO_AXESMODEL__);
    setAxesModel(iAxesmdlUID);

    /* Sets the parent-child relationship between the default Figure and Axes */
    setGraphicObjectRelationship(iFiguremdlUID, iAxesmdlUID);

    /* Axes Model properties */

    result = InitAxesModel();

    if (result < 0)
    {
        strcpy(error_message, _("Default axes cannot be created.\n"));
        return 0;
    }

    /* Margins and clip region */

    margins[0] = 0.125;
    margins[1] = 0.125;
    margins[2] = 0.125;
    margins[3] = 0.125;

    setGraphicObjectProperty(iAxesmdlUID, __GO_MARGINS__, margins, jni_double_vector, 4);

    clipRegion[0] = 0.0;
    clipRegion[1] = 0.0;
    clipRegion[2] = 0.0;
    clipRegion[3] = 0.0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    clipRegionSet = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    /*
     * Specifies that no high-level drawing function has been called yet.
     */
    firstPlot = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

#if 0
    ppaxesmdl->FirstPlot = TRUE;
#endif

    return 1;
}

/*
 * This function has been adapted to the MVC framework, using the
 * MVC's property set/get calls.
 */

int InitAxesModel()
{
    int cubeScaling = 0;
    int logFlag = 0;
    int ticksColor = 0;
    int axisLocation = 0;
    int boxType = 0;
    int filled = 0;
    int gridColor = 0;
    int gridPosition = 0;
    int view = 0;
    int axisReverse = 0;
    int axisVisible = 0;
    int defaultNumberTicks = 0;
    int autoTicks = 0;
    int autoSubticks = 0;
    int nbSubticks = 0;
    int hiddenAxisColor = 0;
    int hiddenColor = 0;
    int isoview = 0;
    int visible = 0;
    int *piVisible = &visible;
    int clipState = 0;
    int tightLimits = 0;
    int arcDrawingMethod = 0;
    int labelsFontStyle = 6;
    int labelsFontColor = -1;
    int i = 0;
    double labelsFontSize = 1;
    double axesBounds[4];
    double dataBounds[6];
    double rotationAngles[2];
    double tab[] = { 0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1. }; /* graduations init. tmptab */
    /* z-axis initial ticks locations */
    double tabZTicksLocations[] = { -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };
    char labelBuffer[LABEL_BUFFER_LENGTH];
    char **stringVector = NULL;

    int lineColor = -1;
    int background = -2;
    int foreground = -1;
    double lineWidth = 1.0;

    /* 0: solid */
    int lineStyle = 0;

    int markMode = 0;
    int lineMode = 1;
    int fillMode = 1;
    int markStyle = 0;
    int markSize = 0;

    /* 0: point, 1: tabulated */
    int markSizeUnit = 1;

    int iLabelUID = 0;

    int iFiguremdlUID = getFigureModel();
    int iAxesmdlUID = getAxesModel();

    sciInitGraphicMode(iAxesmdlUID);

    setGraphicObjectProperty(iAxesmdlUID, __GO_BACKGROUND__, &background, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_LINE_COLOR__, &foreground, jni_int, 1);

    setGraphicObjectProperty(iAxesmdlUID, __GO_FONT_SIZE__, &labelsFontSize, jni_double, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_FONT_COLOR__, &labelsFontColor, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_FONT_STYLE__, &labelsFontStyle, jni_int, 1);

    setGraphicObjectProperty(iAxesmdlUID, __GO_LINE_THICKNESS__, &lineWidth, jni_double, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_LINE_STYLE__, &lineStyle, jni_int, 1);

    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_LINE_MODE__, &lineMode, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);

    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);

    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_BACKGROUND__, &background, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_MARK_FOREGROUND__, &foreground, jni_int, 1);

    cubeScaling = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_CUBE_SCALING__, &cubeScaling, jni_bool, 1);

    /* Log flags set to linear for the 3 axes */
    logFlag = 0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);

    ticksColor = -1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_TICKS_COLOR__, &ticksColor, jni_int, 1);

    nbSubticks = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);

    /* 0 corresponds to bottom position */
    axisLocation = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_LOCATION__, &axisLocation, jni_int, 1);

    /* 4 corresponds to left position */
    axisLocation = 4;
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_LOCATION__, &axisLocation, jni_int, 1);

    /* 0 corresponds to OFF */
    boxType = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

    filled = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_FILLED__, &filled, jni_bool, 1);

    gridColor = -1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);

    /* 0: background */
    setGraphicObjectProperty(iAxesmdlUID, __GO_LINE_COLOR__, &lineColor, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_BACKGROUND__, &background, jni_int, 1);

    gridPosition = 0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_GRID_POSITION__, &gridPosition, jni_int, 1);

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* 0: 2D view */
    view = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_VIEW__, &view, jni_int, 1);

    /* Must be set after VIEW, since setting VIEW to 2D overwrites the 3D rotation angles */
    rotationAngles[0] = 45.0;
    rotationAngles[1] = 215.0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_ROTATION_ANGLES_3D__, rotationAngles, jni_double_vector, 2);

    axisVisible = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

    axisReverse = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_REVERSE__, &axisReverse, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_REVERSE__, &axisReverse, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_REVERSE__, &axisReverse, jni_bool, 1);

    /* Corresponds to the MVC AUTO_SUBTICKS property (!flagNax is equivalent to AUTO_SUBTICKS) */
#if 0
    ppaxesmdl->flagNax = FALSE;
#endif

    autoSubticks = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

    /* To be corrected when the equivalent of flagnax is implemented within the MVC */
    nbSubticks = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);

    /*
     * The code creating default ticks labels and positions should be put
     * into the Model. Ticks positions and locations should be computed using the default data bounds
     * instead of using pre-defined values.
     * Note that the pre-MVC ticks labels creation code is implemented in the C++ Renderer module
     * and should be moved to the Java Model's relevant parts (TicksProperty).
     */

    /*
     * Automatic ticks computation must be activated for the 3 axes before setting ticks values to
     * ensure that the ticks values set are the automatic ticks' ones.
     */
    autoTicks = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

    defaultNumberTicks = 11;

    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_TICKS_LOCATIONS__, tab, jni_double_vector, defaultNumberTicks);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_TICKS_LOCATIONS__, tab, jni_double_vector, defaultNumberTicks);

    stringVector = createStringArray(defaultNumberTicks);

    if (stringVector == NULL)
    {
        return -1;
    }

    /*
     * A proper format should be used (ChoixFormatE function)
     */
    for (i = 0; i < defaultNumberTicks; i++)
    {
        sprintf(labelBuffer, "%.1f", tab[i]);
        stringVector[i] = os_strdup(labelBuffer);

        if (stringVector[i] == NULL)
        {
            destroyStringArray(stringVector, i);
            return -1;
        }
    }

    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);

    /*
     * The same number of ticks is used now for the x,y and z axes.
     * Previously, the z-axis contained only 3 ticks (-1, 0, 1). However, the renderer module was
     * overriding this default number (3) by creating an 11-tick z-axis when required (3D view).
     * Ticks locations and labels are however different for the z-axis (from -1 to +1 instead of 0 to 1).
     */
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_TICKS_LOCATIONS__, tabZTicksLocations, jni_double_vector, defaultNumberTicks);

    /* ChoixFormatE should be used */
    for (i = 0; i < defaultNumberTicks; i++)
    {
        FREE(stringVector[i]);

        sprintf(labelBuffer, "%.1f", tabZTicksLocations[i]);
        stringVector[i] = os_strdup(labelBuffer);

        if (stringVector[i] == NULL)
        {
            destroyStringArray(stringVector, defaultNumberTicks);
            return -1;
        }
    }

    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);

    destroyStringArray(stringVector, defaultNumberTicks);

    /*
     * Indicates the direction of projection (0 for the axis corresponding to the direction,
     * 1 for the others).
     * To be implemented within the MVC if determined to be useful.
     */
#if 0
    ppaxesmdl->project[0] = 1;
    ppaxesmdl->project[1] = 1;
    ppaxesmdl->project[2] = 0;
#endif

    hiddenAxisColor = 4;
    setGraphicObjectProperty(iAxesmdlUID, __GO_HIDDEN_AXIS_COLOR__, &hiddenAxisColor, jni_int, 1);

    hiddenColor = 4;
    setGraphicObjectProperty(iAxesmdlUID, __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);

    isoview = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_ISOVIEW__, &isoview, jni_bool, 1);

    /* Axes bounds set to fill the whole drawing area */
    axesBounds[0] = 0.0;
    axesBounds[1] = 0.0;
    axesBounds[2] = 1.0;
    axesBounds[3] = 1.0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_AXES_BOUNDS__, axesBounds, jni_double_vector, 4);

    /* xmin, xmax */
    dataBounds[0] = 0.0;
    dataBounds[1] = 1.0;
    /* ymin, ymax */
    dataBounds[2] = 0.0;
    dataBounds[3] = 1.0;
    /* zmin, zmax */
    dataBounds[4] = -1.0;
    dataBounds[5] = 1.0;

    setGraphicObjectProperty(iAxesmdlUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    /* visible */
    getGraphicObjectProperty(iFiguremdlUID, __GO_VISIBLE__, jni_bool, (void**)&piVisible);
    setGraphicObjectProperty(iAxesmdlUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* 0: clipping off */
    clipState = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* "real data bounds" and "data bounds" are initially the same */
    setGraphicObjectProperty(iAxesmdlUID, __GO_REAL_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    tightLimits = 0;
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);

    /* Sets the default arc drawing method to lines (1), which is faster */
    arcDrawingMethod = 1;
    setGraphicObjectProperty(iAxesmdlUID, __GO_ARC_DRAWING_METHOD__, &arcDrawingMethod, jni_int, 1);

    /* Creates the Axes model's labels and sets the model as their parent */
    iLabelUID = initLabel(iAxesmdlUID);
    setGraphicObjectProperty(iAxesmdlUID, __GO_TITLE__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iAxesmdlUID);
    setGraphicObjectProperty(iAxesmdlUID, __GO_X_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iAxesmdlUID);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Y_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    iLabelUID = initLabel(iAxesmdlUID);
    setGraphicObjectProperty(iAxesmdlUID, __GO_Z_AXIS_LABEL__, &iLabelUID, jni_int, 1);

    return 0;
}

/*
 * This function has been adapted to the MVC framework.
 * Its code ought to be moved to the Java Model implementation,
 * either within the relevant constructor or an initialization method.
 */

/**sciInitGraphicMode
 * Inits the graphic mode of this object with the default value
 */
int sciInitGraphicMode(int iObjUID)
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    /*
     * The GO_FIGURE block is never reached as InitFigureModel
     * is not called at all (was previously called by
     * the graphicsmodels function).
     */
    if (iType == __GO_FIGURE__)
    {
        /* 3: copy pixel drawing mode */
        int xormode = 3;

        if (isFigureModel(iObjUID))
        {
            /*
             * These 3 properties are not used by the Figure object proper, but
             * rather serve to initialize its children Axes' ones.
             */
            setGraphicObjectProperty(iObjUID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);
        }
    }
    else if (iType == __GO_AXES__)
    {
        /*
         * Same values as the ones from the Figure model. These values were copied from the parent
         * Figure but are for now set using the values below.
         */

        /* autoClear is the logical not of addPlot (autoClear == 0 corresponds to addPlot == TRUE) */
        int autoClear = 0;
        int autoScale = 1;
        int zoom = 0;

        /* 3: copy */
        int xormode = 3;

        if (isAxesModel(iObjUID))
        {
            setGraphicObjectProperty(iObjUID, __GO_AUTO_CLEAR__, &autoClear, jni_bool, 1);
            setGraphicObjectProperty(iObjUID, __GO_AUTO_SCALE__, &autoScale, jni_bool, 1);
            setGraphicObjectProperty(iObjUID, __GO_ZOOM_ENABLED__, &zoom, jni_bool, 1);

            /*
             * Internal state: was possibly used to avoid accessing the parent Figure's pixel drawing mode
             * or may be entirely useless, as pixel drawing mode is associated to the whole Figure.
             * As it has no corresponding MVC property, this call will not set anything.
             */
            setGraphicObjectProperty(iObjUID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);
        }
        /*
         * This block is never reached at all since since the Axes model
         * is now cloned within the MVC via a C call.
         */
        else
        {
            int iTmp = 0;
            int *piTmp = &iTmp;
            int iAxesmdlUID = getAxesModel();

            getGraphicObjectProperty(iAxesmdlUID, __GO_AUTO_CLEAR__, jni_bool, (void **)&piTmp);
            autoClear = iTmp;
            getGraphicObjectProperty(iAxesmdlUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
            autoScale = iTmp;
            getGraphicObjectProperty(iAxesmdlUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&piTmp);
            zoom = iTmp;

            setGraphicObjectProperty(iObjUID, __GO_AUTO_CLEAR__, &autoClear, jni_bool, 1);
            setGraphicObjectProperty(iObjUID, __GO_AUTO_SCALE__, &autoScale, jni_bool, 1);
            setGraphicObjectProperty(iObjUID, __GO_ZOOM_ENABLED__, &zoom, jni_bool, 1);

            /*
             * Internal state: used to avoid accessing the parent's pixel drawing mode
             * obsolete ? Not implemented yet within the MVC
             */

            getGraphicObjectProperty(iAxesmdlUID, __GO_PIXEL_DRAWING_MODE__, jni_bool, (void **)&piTmp);
            xormode = iTmp;

            setGraphicObjectProperty(iObjUID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);
        }
    }

    /* Deactivated */
#if 0
case SCI_TEXT:
case SCI_LEGEND:
case SCI_ARC:
case SCI_SEGS:
case SCI_FEC:
case SCI_GRAYPLOT:
case SCI_POLYLINE:
case SCI_RECTANGLE:
case SCI_SURFACE:
case SCI_AXES:
case SCI_AGREG:
case SCI_LABEL:                /* F.Leray 28.05.04 */
case SCI_UIMENU:
default:
    sciprint(_("This object has not any graphic mode\n"));
    return -1;
    break;
#endif

    return 0;
}

/*---------------------------------------------------------------------------------*/
/* allocate and set a new label to default values */
int initLabel(int iParentObjUID)
{
    int iNewLabel = NULL;
    int iHidden = 1;
    int autoPosition = 1;

    iNewLabel = createGraphicObject(__GO_LABEL__);

    /* Hide Label as they are non explicit children */
    setGraphicObjectProperty(iNewLabel, __GO_HIDDEN__, &iHidden, jni_bool, 1);
    setGraphicObjectProperty(iNewLabel, __GO_AUTO_POSITION__, &autoPosition, jni_bool, 1);
    setGraphicObjectProperty(iNewLabel, __GO_AUTO_ROTATION__, &autoPosition, jni_bool, 1);

    /* Sets the label's parent */
    setGraphicObjectRelationship(iParentObjUID, iNewLabel);

    cloneGraphicContext(iParentObjUID, iNewLabel);
    cloneFontContext(iParentObjUID, iNewLabel);

    return iNewLabel;
}
/*---------------------------------------------------------------------------------*/
