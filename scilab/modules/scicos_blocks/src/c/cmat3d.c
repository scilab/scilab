/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "sci_malloc.h"
#include "core_math.h"
#include "elementary_functions.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"
#include "os_string.h"

#include "FigureList.h"
#include "BuildObjects.h"
#include "AxesModel.h"
/*****************************************************************************
 * Internal container structure
 ****************************************************************************/

/**
 * Container structure
 */
typedef struct
{
    struct
    {
        int cachedFigureUID;
        int cachedAxeUID;
        int cachedPlot3dUID;
    } scope;
} sco_data;

/**
 * Get (and allocate on demand) the internal data used on this scope
 * \param block the block
 * \return the scope data
 */
static sco_data *getScoData(scicos_block * block);

/**
 * Release any internal data
 *
 * \param block the block
 */
static void freeScoData(scicos_block * block);

/**
 * Push the block data to the polyline
 *
 * \param data the data to push
 *
 */
static BOOL pushData(scicos_block * block, double *data);

/*****************************************************************************
 * Graphics utils
 ****************************************************************************/

/**
 * Get (and allocate on demand) the figure associated with the block
 * \param block the block
 * \return a valid figure UID or NULL on error
 */
static int getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param iFigureUID the parent figure UID
 * \param block the block
 * \return a valid axe UID or NULL on error
 */
static int getAxe(int iFigureUID, scicos_block * block);

/**
 * Get (and allocate on demand) the plot3d
 *
 * \param iAxeUID the parent axe UID
 * \param block the block
 * \return a valid plot3d UID or NULL on error
 */
static int getPlot3d(int iAxeUID, scicos_block * block);

/**
 * Set the plot3d and axes bounds
 *
 * \param block the block
 * \param iAxeUID the axe
 * \param pPlot3dUID the plot3d
 */
static BOOL setBounds(scicos_block * block, int iAxeUID, int iPlot3dUID);

/**
 * Set the plot3d settings
 *
 * \param pPlot3dUID the plot3d
 */
static BOOL setPlot3dSettings(int iPlot3dUID);

/**
 * Set the plot3d default values
 *
 * \param block the block
 * \param pPlot3dUID the plot3d
 */
static BOOL setDefaultValues(scicos_block * block, int iPlot3dUID);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cmatview(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cmat3d(scicos_block * block, scicos_flag flag)
{
    int iFigureUID;

    double *u;
    sco_data *sco;

    BOOL result;

    switch (flag)
    {

        case Initialization:
            sco = getScoData(block);
            if (sco == NULL)
            {
                set_block_error(-5);
                break;
            }
            iFigureUID = getFigure(block);
            if (iFigureUID == 0)
            {
                // allocation error
                set_block_error(-5);
                break;
            }
            break;

        case StateUpdate:
            iFigureUID = getFigure(block);
            if (iFigureUID == 0)
            {
                // allocation error
                set_block_error(-5);
                break;
            }

            u = GetRealInPortPtrs(block, 1);

            result = pushData(block, u);
            if (result == FALSE)
            {
                Coserror("%s: unable to push some data.", "cmatview");
                break;
            }
            break;

        case Ending:
            freeScoData(block);
            break;

        default:
            break;
    }
}

/*-------------------------------------------------------------------------*/

/*****************************************************************************
 *
 * Container management
 *
 ****************************************************************************/

static sco_data *getScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
        {
            goto error_handler_sco;
        }

        sco->scope.cachedFigureUID = 0;
        sco->scope.cachedAxeUID = 0;
        sco->scope.cachedPlot3dUID = 0;

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);

    if (sco != NULL)
    {
        FREE(sco);
        *(block->work) = NULL;
    }
}

static BOOL pushData(scicos_block * block, double *data)
{
    int iFigureUID;
    int iAxeUID;
    int iPlot3dUID;

    BOOL result;

    int m, n;

    iFigureUID = getFigure(block);
    iAxeUID = getAxe(iFigureUID, block);
    iPlot3dUID = getPlot3d(iAxeUID, block);

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    result = setGraphicObjectProperty(iPlot3dUID, __GO_DATA_MODEL_Z__, data, jni_double_vector, m * n);

    return result;
}

/*****************************************************************************
 *
 * Graphic utils
 *
 ****************************************************************************/

/*****************************************************************************
 *
 * Graphic
 *
 ****************************************************************************/
static int getFigure(scicos_block * block)
{
    signed int figNum;
    int iFigureUID;
    int iAxe;
    int i__1 = 1;
    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }


    // fast path for an existing object
    if (sco->scope.cachedFigureUID)
    {
        return sco->scope.cachedFigureUID;
    }

    figNum = block->ipar[0];

    // with a negative id, use the block number indexed from a constant.
    if (figNum < 0)
    {
        figNum = 20000 + get_block_number();
    }

    iFigureUID = getFigureFromIndex(figNum);
    // create on demand
    if (iFigureUID == 0)
    {
        iFigureUID = createNewFigureWithAxes();
        setGraphicObjectProperty(iFigureUID, __GO_ID__, &figNum, jni_int, 1);

        // the stored uid is a reference to the figure map, not to the current figure
        iFigureUID = getFigureFromIndex(figNum);
        sco->scope.cachedFigureUID = iFigureUID;

        setGraphicObjectProperty(iFigureUID, __GO_COLORMAP__, block->rpar, jni_double_vector, block->ipar[2]);

        // allocate the axes through the getter
        iAxe = getAxe(iFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(iAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(iAxe, __GO_Y_AXIS_LABEL__, "y");
        setLabel(iAxe, __GO_Z_AXIS_LABEL__, "z");

        setGraphicObjectProperty(iAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
        setGraphicObjectProperty(iAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);
        setGraphicObjectProperty(iAxe, __GO_Z_AXIS_VISIBLE__, &i__1, jni_bool, 1);
    }

    if (sco->scope.cachedFigureUID == 0)
    {
        sco->scope.cachedFigureUID = iFigureUID;
    }
    return iFigureUID;
}

static int getAxe(int iFigureUID, scicos_block * block)
{
    int iAxe;
    int i__1 = 1;
    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (sco->scope.cachedAxeUID)
    {
        return sco->scope.cachedAxeUID;
    }

    iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, 0);

    /*
     * Allocate if necessary
     */
    if (iAxe == 0)
    {
        cloneAxesModel(iFigureUID);
        iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, 0);
    }

    if (iAxe != 0)
    {
        setGraphicObjectProperty(iAxe, __GO_BOX_TYPE__, &i__1, jni_int, 1);

        getPlot3d(iAxe, block);
    }
    else
    {
        return 0;
    }

    /*
     * then cache with local storage
     */
    sco->scope.cachedAxeUID = iAxe;
    return sco->scope.cachedAxeUID;
}

static int getPlot3d(int iAxeUID, scicos_block * block)
{
    int iPlot3d;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (sco->scope.cachedPlot3dUID)
    {
        return sco->scope.cachedPlot3dUID;
    }

    iPlot3d = findChildWithKindAt(iAxeUID, __GO_PLOT3D__, 0);

    /*
     * Allocate if necessary
     */
    if (iPlot3d == 0)
    {
        iPlot3d = createGraphicObject(__GO_PLOT3D__);

        if (iPlot3d != 0)
        {
            createDataObject(iPlot3d, __GO_PLOT3D__);
            setGraphicObjectRelationship(iAxeUID, iPlot3d);
        }
        else
        {
            return 0;
        }
    }

    /*
     * Setup on first access
     */
    setBounds(block, iAxeUID, iPlot3d);
    setPlot3dSettings(iPlot3d);
    setDefaultValues(block, iPlot3d);

    {
        int iClipState = 1; //on
        setGraphicObjectProperty(iPlot3d, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
    }

    /*
     * then cache with a local storage
     */
    sco->scope.cachedPlot3dUID = iPlot3d;
    return sco->scope.cachedPlot3dUID;
}

static BOOL setBounds(scicos_block * block, int iAxeUID, int iPlot3dUID)
{
    BOOL result;

    int gridSize[4];
    double dataBounds[6];
    double rotationAngle[2];

    int m, n;
    int colormapLen;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    gridSize[0] = 1;
    gridSize[1] = m;
    gridSize[2] = 1;
    gridSize[3] = n;

    colormapLen = block->ipar[3];
    if (colormapLen == 1)
    {
        dataBounds[0] = (double) 0;  // xMin
        dataBounds[1] = (double) m;  // xMax
        dataBounds[2] = (double) 0;  // yMin
        dataBounds[3] = (double) n;  // yMax
    }
    else
    {
        dataBounds[0] = block->rpar[colormapLen + 0];   // xMin
        dataBounds[1] = block->rpar[colormapLen + 1];   // xMax
        dataBounds[2] = block->rpar[colormapLen + 2];   // yMin
        dataBounds[3] = block->rpar[colormapLen + 3];   // yMax
    }

    dataBounds[4] = (double)block->ipar[0]; // zMin
    dataBounds[5] = (double)block->ipar[1]; // zMax

    rotationAngle[0] = 50;      // alpha
    rotationAngle[1] = 280;     // theta

    result = setGraphicObjectProperty(iPlot3dUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iAxeUID, __GO_ROTATION_ANGLES__, rotationAngle, jni_double_vector, 2);

    return result;
}

static BOOL setPlot3dSettings(int iPlot3dUID)
{
    int i__1 = 1;
    double d__1 = 1.0;
    int i__2 = 2;
    int i__4 = 4;

    BOOL result;

    result = setGraphicObjectProperty(iPlot3dUID, __GO_SURFACE_MODE__, &i__1, jni_bool, 1);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iPlot3dUID, __GO_LINE_THICKNESS__, &d__1, jni_double, 1);
    if (result == FALSE)
    {
        return result;
    }

    result = setGraphicObjectProperty(iPlot3dUID, __GO_COLOR_MODE__, &i__2, jni_int, 1);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iPlot3dUID, __GO_COLOR_FLAG__, &i__1, jni_int, 1);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iPlot3dUID, __GO_HIDDEN_COLOR__, &i__4, jni_int, 1);
    if (result == FALSE)
    {
        return result;
    }

    result = setGraphicObjectProperty(iPlot3dUID, __GO_CLIP_STATE__, &i__1, jni_int, 1);

    return result;
}

static BOOL setDefaultValues(scicos_block * block, int iPlot3dUID)
{
    int m, n, len;
    int i;
    double *values;

    BOOL result;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    /*
     * Share the same memory for 0 allocation (z) and incremented index (x and y)
     */
    values = (double *)CALLOC(m * n, sizeof(double));
    if (values == NULL)
    {
        return FALSE;
    }
    result = setGraphicObjectProperty(iPlot3dUID, __GO_DATA_MODEL_Z__, values, jni_double_vector, m * n);
    if (result == FALSE)
    {
        goto local_return;
    }

    len = Max(m, n);
    for (i = 1; i <= len; i++)
    {
        values[i] = (double) i;
    }

    result = setGraphicObjectProperty(iPlot3dUID, __GO_DATA_MODEL_X__, values, jni_double_vector, m);
    if (result == FALSE)
    {
        goto local_return;
    }
    result = setGraphicObjectProperty(iPlot3dUID, __GO_DATA_MODEL_Y__, values, jni_double_vector, n);

local_return:
    FREE(values);
    return result;
}

