/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "MALLOC.h"
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
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

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
        char const* cachedFigureUID;
        char *cachedAxeUID;
        char *cachedGrayplotUID;
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
static char const* getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param pFigureUID the parent figure UID
 * \param block the block
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char const* pFigureUID, scicos_block * block);

/**
 * Get (and allocate on demand) the grayplot
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \return a valid grayplot UID or NULL on error
 */
static char *getGrayplot(char *pAxeUID, scicos_block * block);

/**
 * Set the grayplot and axes bounds
 *
 * \param block the block
 * \param pAxeUID the axe
 * \param pGrayplotUID the grayplot
 */
static BOOL setBounds(scicos_block * block, char *pAxeUID, char *pGrayplotUID);

/**
 * Set the grayplot default values
 *
 * \param block the block
 * \param pGrayplotUID the grayplot
 */
static BOOL setDefaultValues(scicos_block * block, char *pGrayplotUID);

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
    char const* pFigureUID;

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
            pFigureUID = getFigure(block);
            if (pFigureUID == NULL)
            {
                // allocation error
                set_block_error(-5);
                break;
            }
            break;

        case StateUpdate:
            pFigureUID = getFigure(block);
            if (pFigureUID == NULL)
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
            goto error_handler_sco;

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;
        sco->scope.cachedGrayplotUID = NULL;

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
        FREE(sco->scope.cachedAxeUID);
        FREE(sco->scope.cachedGrayplotUID);

        FREE(sco);
        *(block->work) = NULL;
    }
}

static BOOL pushData(scicos_block * block, double *data)
{
    char const* pFigureUID;
    char *pAxeUID;
    char *pGrayplotUID;

    BOOL result;
    int i;

    int m, n;
    double alpha, beta;
    double *scaledData;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pGrayplotUID = getGrayplot(pAxeUID, block);

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

    result = setGraphicObjectProperty(pGrayplotUID, __GO_DATA_MODEL_Z__, scaledData, jni_double_vector, m * n);
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

static char const* getFigure(scicos_block * block)
{
    signed int figNum;
    char const* pFigureUID = NULL;
    char *pAxe = NULL;
    int i__1 = 1;
    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (sco->scope.cachedFigureUID != NULL)
    {
        return sco->scope.cachedFigureUID;
    }

    figNum = block->ipar[0];

    // with a negative id, use the block number indexed from a constant.
    if (figNum < 0)
    {
        figNum = 20000 + get_block_number();
    }

    pFigureUID = getFigureFromIndex(figNum);
    // create on demand
    if (pFigureUID == NULL)
    {
        pFigureUID = createNewFigureWithAxes();
        setGraphicObjectProperty(pFigureUID, __GO_ID__, &figNum, jni_int, 1);

        // the stored uid is a reference to the figure map, not to the current figure
        pFigureUID = getFigureFromIndex(figNum);
        sco->scope.cachedFigureUID = pFigureUID;

        setGraphicObjectProperty(pFigureUID, __GO_COLORMAP__, &block->rpar[2], jni_double_vector, block->ipar[2]);

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(pAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");

        setGraphicObjectProperty(pAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
        setGraphicObjectProperty(pAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);
    }

    if (pFigureUID != NULL && sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char const* pFigureUID, scicos_block * block)
{
    char *pAxe;
    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (sco->scope.cachedAxeUID != NULL)
    {
        return sco->scope.cachedAxeUID;
    }

    pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, 0);

    /*
     * Allocate if necessary
     */
    if (pAxe == NULL)
    {
        cloneAxesModel(pFigureUID);
        pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, 0);
    }

    /*
     * Setup on first access
     */
    if (pAxe != NULL)
    {
        getGrayplot(pAxe, block);
    }


    /*
     * then cache with local storage
     */
    if (pAxe != NULL && sco->scope.cachedAxeUID == NULL)
    {
        sco->scope.cachedAxeUID = strdup(pAxe);
        releaseGraphicObjectProperty(__GO_PARENT__, pAxe, jni_string, 1);
    }
    return sco->scope.cachedAxeUID;
}

static char *getGrayplot(char *pAxeUID, scicos_block * block)
{
    char *pGrayplot;
    int i__0 = 0;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (sco->scope.cachedGrayplotUID != NULL)
    {
        return sco->scope.cachedGrayplotUID;
    }

    pGrayplot = findChildWithKindAt(pAxeUID, __GO_GRAYPLOT__, 0);

    /*
     * Allocate if necessary
     */
    if (pGrayplot == NULL)
    {
        pGrayplot = createGraphicObject(__GO_GRAYPLOT__);

        if (pGrayplot != NULL)
        {
            createDataObject(pGrayplot, __GO_GRAYPLOT__);
            setGraphicObjectRelationship(pAxeUID, pGrayplot);
        }
    }

    /*
     * Setup on first access
     */
    if (pGrayplot != NULL)
    {

        setGraphicObjectProperty(pGrayplot, __GO_DATA_MAPPING__, &i__0, jni_int, 1);
        setBounds(block, pAxeUID, pGrayplot);
        setDefaultValues(block, pGrayplot);

        {
            int iClipState = 1; //on
            setGraphicObjectProperty(pGrayplot, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
        }
    }

    /*
     * then cache with a local storage
     */
    if (pGrayplot != NULL && sco->scope.cachedGrayplotUID == NULL)
    {
        sco->scope.cachedGrayplotUID = strdup(pGrayplot);
        releaseGraphicObjectProperty(__GO_PARENT__, pGrayplot, jni_string, 1);
    }
    return sco->scope.cachedGrayplotUID;
}

static BOOL setBounds(scicos_block * block, char *pAxeUID, char *pGrayplotUID)
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

    result = setGraphicObjectProperty(pGrayplotUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    result &= setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

    return result;
}

static BOOL setDefaultValues(scicos_block * block, char *pGrayplotUID)
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

    result = setGraphicObjectProperty(pGrayplotUID, __GO_DATA_MODEL_Z__, values, jni_double_vector, m * n);

    for (i = 1; i <= len; i++)
    {
        values[i] = (double)i;
    }
    result &= setGraphicObjectProperty(pGrayplotUID, __GO_DATA_MODEL_X__, values, jni_double_vector, m);
    result &= setGraphicObjectProperty(pGrayplotUID, __GO_DATA_MODEL_Y__, values, jni_double_vector, n);

    FREE(values);
    return result;
}
