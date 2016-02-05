/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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

#include <math.h>
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "sci_malloc.h"
#include "elementary_functions.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
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
        double *ballsSize;
        double **data;
    } internal;

    struct
    {
        int cachedFigureUID;
        int cachedAxeUID;
        int* cachedArcsUIDs;
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
static int getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param iFigureUID the parent figure UID
 * \param block the block
 * \param input the current input index (0-indexed)
 * \return a valid axe UID or NULL on error
 */
static int getAxe(int iFigureUID, scicos_block * block);

/**
 * Get (and allocate on demand) the arc associated with the row
 *
 * \param iAxeUID the parent axe UID
 * \param block the block
 * \param row the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static int getArc(int iAxeUID, scicos_block * block, int row);

/**
 * Set the bounds
 *
 * \param block the block
 * \param iAxeUID the axe id
 */
static BOOL setBounds(scicos_block * block, int iAxeUID);

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
    int iFigureUID;

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
            iFigureUID = getFigure(block);
            if (iFigureUID == 0)
            {
                // allocation error
                set_block_error(-5);
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
        {
            goto error_handler_sco;
        }

        sco->internal.ballsSize = (double *)CALLOC(block->insz[0], sizeof(double));
        if (sco->internal.ballsSize == NULL)
        {
            goto error_handler_ballsSize;
        }
        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.ballsSize[i] = block->z[6 * i + 2];
        }

        sco->internal.data = (double **)CALLOC(block->insz[0], sizeof(double *));
        if (sco->internal.data == NULL)
        {
            goto error_handler_data;
        }

        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.data[i] = (double *)CALLOC(3, sizeof(double));
            if (sco->internal.data[i] == NULL)
            {
                goto error_handler_data_i;
            }
        }

        sco->scope.cachedFigureUID = 0;
        sco->scope.cachedAxeUID = 0;

        sco->scope.cachedArcsUIDs = (int*)CALLOC(block->insz[0], sizeof(int));

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
        FREE(sco->scope.cachedArcsUIDs);

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
    int iFigureUID;
    int iAxeUID;
    int iArcUID;

    double *upperLeftPoint;
    sco_data *sco;

    iFigureUID = getFigure(block);
    iAxeUID = getAxe(iFigureUID, block);
    iArcUID = getArc(iAxeUID, block, row);

    sco = getScoData(block);
    if (sco == NULL)
    {
        return FALSE;
    }

    upperLeftPoint = sco->internal.data[row];
    return setGraphicObjectProperty(iArcUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);
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
    int iFigureUID = 0;
    int iAxe = 0;
    int i__1 = 1;
    BOOL b_true = TRUE;

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

        // allocate the axes through the getter
        iAxe = getAxe(iFigureUID, block);

        /*
         * Setup according to block settings
         */
        setGraphicObjectProperty(iAxe, __GO_BOX_TYPE__, &i__1, jni_int, 1);
        setGraphicObjectProperty(iAxe, __GO_ISOVIEW__, &b_true, jni_bool, 1);

        setBounds(block, iAxe);
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
    int i;

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
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[0]; i++)
        {
            getArc(iAxe, block, i);
        }
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

static int getArc(int iAxeUID, scicos_block * block, int row)
{
    static double d__0 = 0.0;
    static double d__2PI = 2 * M_PI;
    static BOOL b__true = TRUE;

    int iArc;
    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL || sco->scope.cachedArcsUIDs == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (sco->scope.cachedArcsUIDs[row])
    {
        return sco->scope.cachedArcsUIDs[row];
    }

    iArc = findChildWithKindAt(iAxeUID, __GO_ARC__, row);

    /*
     * Allocate if necessary
     */
    if (iArc == 0)
    {
        iArc = createGraphicObject(__GO_ARC__);

        if (iArc != 0)
        {
            createDataObject(iArc, __GO_ARC__);
            setGraphicObjectRelationship(iAxeUID, iArc);
        }
        else
        {
            return 0;
        }
    }

    /*
     * Setup on first access
     */
    setGraphicObjectProperty(iArc, __GO_START_ANGLE__, &d__0, jni_double, 1);
    setGraphicObjectProperty(iArc, __GO_END_ANGLE__, &d__2PI, jni_double, 1);

    color = block->ipar[2 + row];
    setGraphicObjectProperty(iArc, __GO_BACKGROUND__, &color, jni_int, 1);

    setGraphicObjectProperty(iArc, __GO_WIDTH__, &sco->internal.ballsSize[row], jni_double, 1);
    setGraphicObjectProperty(iArc, __GO_HEIGHT__, &sco->internal.ballsSize[row], jni_double, 1);

    setGraphicObjectProperty(iArc, __GO_FILL_MODE__, &b__true, jni_bool, 1);

    {
        int iClipState = 1; //on
        setGraphicObjectProperty(iArc, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
    }

    /*
     * then cache with local storage
     */
    sco->scope.cachedArcsUIDs[row] = iArc;
    return sco->scope.cachedArcsUIDs[row];
}

static BOOL setBounds(scicos_block * block, int iAxeUID)
{
    double dataBounds[6];

    dataBounds[0] = block->rpar[0]; // xMin
    dataBounds[1] = block->rpar[1]; // xMax
    dataBounds[2] = block->rpar[2]; // yMin
    dataBounds[3] = block->rpar[3]; // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    return setGraphicObjectProperty(iAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
