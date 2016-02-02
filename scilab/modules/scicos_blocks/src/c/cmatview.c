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
#include "math.h"

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
        int cachedGrayplotUID;
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
 * Get (and allocate on demand) the grayplot
 *
 * \param iAxeUID the parent axe UID
 * \param block the block
 * \return a valid grayplot UID or NULL on error
 */
static int getGrayplot(int iAxeUID, scicos_block * block);

/**
 * Set the grayplot and axes bounds
 *
 * \param block the block
 * \param iAxeUID the axe
 * \param pGrayplotUID the grayplot
 */
static BOOL setBounds(scicos_block * block, int iAxeUID, int iGrayplotUID);

/**
 * Set the grayplot default values
 *
 * \param block the block
 * \param pGrayplotUID the grayplot
 */
static BOOL setDefaultValues(scicos_block * block, int iGrayplotUID);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cmatview(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cmatview(scicos_block * block, scicos_flag flag)
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
        sco->scope.cachedGrayplotUID = 0;

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
    int iGrayplotUID;

    BOOL result;
    int i;

    int m, n;
    double alpha, beta;
    double *scaledData;

    iFigureUID = getFigure(block);
    iAxeUID = getAxe(iFigureUID, block);
    iGrayplotUID = getGrayplot(iAxeUID, block);

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);
    if (m * n <= 0)
    {
        set_block_error(-5);
        return FALSE;
    }

    /*
     * Scale the data
     */
    alpha = block->rpar[0];
    beta = block->rpar[1];

    scaledData = (double *)MALLOC(m * n * sizeof(double));
    if (scaledData == NULL)
    {
        return FALSE;
    }

    for (i = 0; i < m * n; i++)
    {
        scaledData[i] = floor(alpha * data[i] + beta);
    }

    result = setGraphicObjectProperty(iGrayplotUID, __GO_DATA_MODEL_Z__, scaledData, jni_double_vector, m * n);
    FREE(scaledData);

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

        setGraphicObjectProperty(iFigureUID, __GO_COLORMAP__, &block->rpar[2], jni_double_vector, block->ipar[2]);

        // allocate the axes through the getter
        iAxe = getAxe(iFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(iAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(iAxe, __GO_Y_AXIS_LABEL__, "y");

        setGraphicObjectProperty(iAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
        setGraphicObjectProperty(iAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);
    }

    if (iFigureUID != 0 && sco->scope.cachedFigureUID == 0)
    {
        sco->scope.cachedFigureUID = iFigureUID;
    }
    return iFigureUID;
}

static int getAxe(int iFigureUID, scicos_block * block)
{
    int iAxe;
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

    /*
     * Setup on first access
     */
    if (iAxe != 0)
    {
        getGrayplot(iAxe, block);
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

static int getGrayplot(int iAxeUID, scicos_block * block)
{
    int iGrayplot;
    int i__0 = 0;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (sco->scope.cachedGrayplotUID)
    {
        return sco->scope.cachedGrayplotUID;
    }

    iGrayplot = findChildWithKindAt(iAxeUID, __GO_GRAYPLOT__, 0);

    /*
     * Allocate if necessary
     */
    if (iGrayplot == 0)
    {
        iGrayplot = createGraphicObject(__GO_GRAYPLOT__);

        if (iGrayplot != 0)
        {
            createDataObject(iGrayplot, __GO_GRAYPLOT__);
            setGraphicObjectRelationship(iAxeUID, iGrayplot);
        }
        else
        {
            return 0;
        }
    }

    /*
     * Setup on first access
     */
    setGraphicObjectProperty(iGrayplot, __GO_DATA_MAPPING__, &i__0, jni_int, 1);
    setBounds(block, iAxeUID, iGrayplot);
    setDefaultValues(block, iGrayplot);

    {
        int iClipState = 1; //on
        setGraphicObjectProperty(iGrayplot, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
    }

    /*
     * then cache with a local storage
     */
    sco->scope.cachedGrayplotUID = iGrayplot;
    return sco->scope.cachedGrayplotUID;
}

static BOOL setBounds(scicos_block * block, int iAxeUID, int iGrayplotUID)
{
    BOOL result;

    int gridSize[4];
    double dataBounds[6];

    int m, n;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    gridSize[0] = m;
    gridSize[1] = 1;
    gridSize[2] = n;
    gridSize[3] = 1;

    dataBounds[0] = 0;          // xMin
    dataBounds[1] = (double)m;  // xMax
    dataBounds[2] = 0;          // yMin
    dataBounds[3] = (double)n;  // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    result = setGraphicObjectProperty(iGrayplotUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    if (result == FALSE)
    {
        return result;
    }
    result = setGraphicObjectProperty(iAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
    if (result == FALSE)
    {
        return result;
    }

    return result;
}

static BOOL setDefaultValues(scicos_block * block, int iGrayplotUID)
{
    int m, n, len;
    int i;
    double *values;

    BOOL result;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    len = Max(m, n);

    values = (double *)CALLOC(n * m, sizeof(double));
    if (values == NULL)
    {
        return FALSE;
    }

    result = setGraphicObjectProperty(iGrayplotUID, __GO_DATA_MODEL_Z__, values, jni_double_vector, m * n);
    if (result == FALSE)
    {
        goto local_return;
    }

    for (i = 1; i <= len; i++)
    {
        values[i] = (double)i;
    }

    result = setGraphicObjectProperty(iGrayplotUID, __GO_DATA_MODEL_X__, values, jni_double_vector, m);
    if (result == FALSE)
    {
        goto local_return;
    }
    result = setGraphicObjectProperty(iGrayplotUID, __GO_DATA_MODEL_Y__, values, jni_double_vector, n);
    if (result == FALSE)
    {
        goto local_return;
    }

local_return:
    FREE(values);
    return result;
}
