/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "MALLOC.h"
#include "elementary_functions.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"

#include "scicos_block4.h"
#include "scicos.h"

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
        double *ballsSize;
        double **data;
    } internal;

    struct
    {
        char const* cachedFigureUID;
        char *cachedAxeUID;
        char **cachedArcsUIDs;
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
 * Append the data to the current data
 *
 * \param block the block
 * \param x x data
 * \param y y data
 */
static void appendData(scicos_block * block, double *x, double *y);

/**
 * Push the block data to the polyline
 *
 * \param block the block
 * \param row the selected row
 *
 */
static BOOL pushData(scicos_block * block, int row);

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
 * \param input the current input index (0-indexed)
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char const* pFigureUID, scicos_block * block);

/**
 * Get (and allocate on demand) the arc associated with the row
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \param row the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static char *getArc(char *pAxeUID, scicos_block * block, int row);

/**
 * Set the bounds
 *
 * \param block the block
 */
static BOOL setBounds(scicos_block * block);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void bouncexy(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void bouncexy(scicos_block * block, scicos_flag flag)
{
    char const* pFigureUID;

    sco_data *sco;

    int j;
    BOOL result;

    switch (flag)
    {

        case Initialization:
            sco = getScoData(block);
            if (sco == NULL)
            {
                set_block_error(-5);
            }
            pFigureUID = getFigure(block);
            if (pFigureUID == NULL)
            {
                // allocation error
                set_block_error(-5);
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

            appendData(block, (double *)block->inptr[0], (double *)block->inptr[1]);
            for (j = 0; j < block->insz[0]; j++)
            {
                result = pushData(block, j);
                if (result == FALSE)
                {
                    Coserror("%s: unable to push some data.", "bouncexy");
                    break;
                }
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
    int i, j;

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
            goto error_handler_sco;

        sco->internal.ballsSize = (double *)CALLOC(block->insz[0], sizeof(double));
        if (sco->internal.ballsSize == NULL)
            goto error_handler_ballsSize;
        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.ballsSize[i] = block->z[6 * i + 2];
        }

        sco->internal.data = (double **)CALLOC(block->insz[0], sizeof(double *));
        if (sco->internal.data == NULL)
            goto error_handler_data;

        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.data[i] = (double *)CALLOC(3, sizeof(double));
            if (sco->internal.data[i] == NULL)
                goto error_handler_data_i;
        }

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;

        sco->scope.cachedArcsUIDs = (char **)CALLOC(block->insz[0], sizeof(char *));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_data_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.data[i]);
    }
    FREE(sco->internal.data);
error_handler_data:
    FREE(sco->internal.ballsSize);
error_handler_ballsSize:
    FREE(sco);
error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i;

    if (sco != NULL)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            FREE(sco->internal.data[i]);
        }

        FREE(sco->internal.data);
        FREE(sco->internal.ballsSize);

        for (i = 0; i < block->insz[0]; i++)
        {
            FREE(sco->scope.cachedArcsUIDs[i]);
            sco->scope.cachedArcsUIDs[i] = NULL;
        }
        FREE(sco->scope.cachedAxeUID);
        sco->scope.cachedAxeUID = NULL;

        FREE(sco);
        *(block->work) = NULL;
    }
}

static void appendData(scicos_block * block, double *x, double *y)
{
    int i;
    double *upperLeftPoint;
    double ballSize;
    sco_data *sco = (sco_data *) * (block->work);

    /*
     * Update data
     */
    if (sco != NULL)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            upperLeftPoint = sco->internal.data[i];
            ballSize = sco->internal.ballsSize[i];

            upperLeftPoint[0] = x[i] - (ballSize / 2);  // x
            upperLeftPoint[1] = y[i] + (ballSize / 2);  // y
            upperLeftPoint[2] = 0;  // z
        }
    }
}

static BOOL pushData(scicos_block * block, int row)
{
    char const* pFigureUID;
    char *pAxeUID;
    char *pArcUID;

    double *upperLeftPoint;
    sco_data *sco;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pArcUID = getArc(pAxeUID, block, row);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;

    upperLeftPoint = sco->internal.data[row];
    return setGraphicObjectProperty(pArcUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);
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
    BOOL b_true = TRUE;

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

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setGraphicObjectProperty(pAxe, __GO_BOX_TYPE__, &i__1, jni_int, 1);
        setGraphicObjectProperty(pAxe, __GO_ISOVIEW__, &b_true, jni_bool, 1);

        setBounds(block);
    }

    if (sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char const* pFigureUID, scicos_block * block)
{
    char *pAxe;
    int i;

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
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[0]; i++)
        {
            getArc(pAxe, block, i);
        }
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

static char *getArc(char *pAxeUID, scicos_block * block, int row)
{
    static double d__0 = 0.0;
    static double d__2PI = 2 * M_PI;
    static BOOL b__true = TRUE;

    char *pArc;
    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (sco->scope.cachedArcsUIDs != NULL && sco->scope.cachedArcsUIDs[row] != NULL)
    {
        return sco->scope.cachedArcsUIDs[row];
    }

    pArc = findChildWithKindAt(pAxeUID, __GO_ARC__, row);

    /*
     * Allocate if necessary
     */
    if (pArc == NULL)
    {
        pArc = createGraphicObject(__GO_ARC__);

        if (pArc != NULL)
        {
            createDataObject(pArc, __GO_ARC__);
            setGraphicObjectRelationship(pAxeUID, pArc);
        }
    }

    /*
     * Setup on first access
     */
    if (pArc != NULL)
    {
        setGraphicObjectProperty(pArc, __GO_START_ANGLE__, &d__0, jni_double, 1);
        setGraphicObjectProperty(pArc, __GO_END_ANGLE__, &d__2PI, jni_double, 1);

        color = block->ipar[2 + row];
        setGraphicObjectProperty(pArc, __GO_BACKGROUND__, &color, jni_int, 1);

        setGraphicObjectProperty(pArc, __GO_WIDTH__, &sco->internal.ballsSize[row], jni_double, 1);
        setGraphicObjectProperty(pArc, __GO_HEIGHT__, &sco->internal.ballsSize[row], jni_double, 1);

        setGraphicObjectProperty(pArc, __GO_FILL_MODE__, &b__true, jni_bool, 1);

        {
            int iClipState = 1; //on
            setGraphicObjectProperty(pArc, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
        }
    }

    /*
     * then cache with local storage
     */
    if (pArc != NULL && sco->scope.cachedArcsUIDs != NULL && sco->scope.cachedArcsUIDs[row] == NULL)
    {
        sco->scope.cachedArcsUIDs[row] = strdup(pArc);
        releaseGraphicObjectProperty(__GO_PARENT__, pArc, jni_string, 1);
    }
    return sco->scope.cachedArcsUIDs[row];
}

static BOOL setBounds(scicos_block * block)
{
    char const* pFigureUID;
    char *pAxeUID;

    double dataBounds[6];

    dataBounds[0] = block->rpar[0]; // xMin
    dataBounds[1] = block->rpar[1]; // xMax
    dataBounds[2] = block->rpar[2]; // yMin
    dataBounds[3] = block->rpar[3]; // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);

    return setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
