/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - Scilab Enterprises - Clement DAVID
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

#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "sci_malloc.h"
#include "elementary_functions.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"
#include "deleteGraphicObject.h"

#include "CurrentFigure.h"
#include "CurrentObject.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"
#include "os_string.h"

#include "FigureList.h"
#include "BuildObjects.h"
#include "AxesModel.h"

// #include <stdio.h>

// #define LOG(...) printf(__VA_ARGS__)
#define LOG(...)
// #define PUSH_LOG(...) printf(__VA_ARGS__)
#define PUSH_LOG(...)


#define HISTORY_POINTS_THRESHOLD 4096

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
        int *numberOfPoints;
        double ***bufferCoordinates;
        int *maxNumberOfPoints;
        double ***historyCoordinates;
    } internal;

    struct
    {
        int *periodCounter;

        BOOL *disableBufferUpdate;
        int *historyUpdateCounter;

        int cachedFigureUID;
        int* cachedAxeUID;
        int** cachedBufferPolylinesUIDs;
        int** cachedHistoryPolylinesUIDs;
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
 * Realloc the history buffer data
 *
 * \param block the block
 * \param input the selected input
 * \param numberOfPoints realloc to store this number of points
 */
static sco_data *reallocHistoryBuffer(scicos_block * block, int input, int numberOfPoints);

/**
 * Set values into the coordinates buffer.
 *
 * \param block the block
 * \param input the selected input
 * \param coordinates the buffer
 * \param numberOfPoints the number of points to set (actual)
 * \param bufferPoints the buffer size (max)
 * \param t the time to set
 * \param value the value to set
 */
static void setBuffersCoordinates(scicos_block * block, int input, double* coordinates, const int numberOfPoints,
                                  const int bufferPoints, const double t, const double value);

/**
 * Append the data to the current data
 *
 * \param block the block
 * \param input the input (0-indexed)
 * \param t the current time
 * \param data the data to append
 */
static void appendData(scicos_block * block, int input, double t, double *data);

/**
 * Push the block data to the polyline
 *
 * \param block the block
 * \param input the selected input
 * \param row the selected row
 * \param iPolylineUID the polyline uid
 *
 */
static BOOL pushData(scicos_block * block, int input, int row);

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
static int getAxe(int iFigureUID, scicos_block * block, int input);

/**
 * Get (and allocate on demand) the polyline associated with the row
 *
 * \param iAxeUID the parent axe UID
 * \param block the block
 * \param input the current input index (0-indexed)
 * \param row the current row index (0-indexed)
 * \param history get the history polyline
 * \return a valid polyline UID or NULL on error
 */
static int getPolyline(int iAxeUID, scicos_block * block, int input, int row, BOOL history);

/**
 * Delete all the buffer polylines.
 *
 * \param block the block
 */
static void deleteBufferPolylines(scicos_block * block);

/**
 * Set the polylines history size and push the history buffer
 *
 * \param block the block
 * \param input the input port index
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL pushHistory(scicos_block * block, int input, int maxNumberOfPoints);

/**
 * Set the polylines bounds
 *
 * \param block the block
 * \param iAxeUID the axe id
 * \param input the input number
 * \param periodCounter number of past periods since startup
 */
static BOOL setPolylinesBounds(scicos_block * block, int iAxeUID, int input, int periodCounter);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cmscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cmscope(scicos_block * block, scicos_flag flag)
{
    int iFigureUID;

    double t;
    double *u;
    sco_data *sco;

    int i, j;
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

            t = get_scicos_time();
            for (i = 0; i < block->nin; i++)
            {
                u = (double *)block->inptr[i];

                appendData(block, i, t, u);
                for (j = 0; j < block->insz[i]; j++)
                {
                    result = pushData(block, i, j);
                    if (result == FALSE)
                    {
                        Coserror("%s: unable to push some data.", "cmscope");
                        break;
                    }
                }
            }
            break;

        case Ending:
            sco = getScoData(block);
            for (i = 0; i < block->nin; i++)
            {
                sco = reallocHistoryBuffer(block, i, sco->internal.maxNumberOfPoints[i] + sco->internal.numberOfPoints[i]);
                sco->scope.disableBufferUpdate[i] = FALSE;
                sco->scope.historyUpdateCounter[i] = 0;
                pushHistory(block, i, sco->internal.maxNumberOfPoints[i]);
            }
            deleteBufferPolylines(block);
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

        sco->internal.numberOfPoints = (int *) MALLOC(block->nin * sizeof(int));
        if (sco->internal.numberOfPoints == NULL)
        {
            goto error_handler_numberOfPoints;
        }
        sco->internal.maxNumberOfPoints = (int *) MALLOC(block->nin * sizeof(int));
        if (sco->internal.maxNumberOfPoints == NULL)
        {
            goto error_handler_maxNumberOfPoints;
        }

        for (i = 0; i < block->nin; i++)
        {
            // 0 points out of a block->ipar[2] points buffer
            sco->internal.numberOfPoints[i] = 0;
            // 0 points out of a 0 points history
            sco->internal.maxNumberOfPoints[i] = 0;
        }

        sco->internal.bufferCoordinates = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.bufferCoordinates == NULL)
        {
            goto error_handler_bufferCoordinates;
        }

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.bufferCoordinates[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.bufferCoordinates[i] == NULL)
            {
                goto error_handler_bufferCoordinates_i;
            }
        }
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                sco->internal.bufferCoordinates[i][j] = (double *)CALLOC(3 * block->ipar[2], sizeof(double));

                if (sco->internal.bufferCoordinates[i][j] == NULL)
                {
                    goto error_handler_bufferCoordinates_ij;
                }
            }
        }

        sco->internal.historyCoordinates = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.historyCoordinates == NULL)
        {
            goto error_handler_historyCoordinates;
        }

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.historyCoordinates[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.historyCoordinates[i] == NULL)
            {
                goto error_handler_historyCoordinates_i;
            }
        }

        sco->scope.periodCounter = (int *) CALLOC(block->nin, sizeof(int));
        if (sco->scope.periodCounter == NULL)
        {
            goto error_handler_periodCounter;
        }

        sco->scope.disableBufferUpdate = (BOOL *) CALLOC(block->nin, sizeof(BOOL));
        if (sco->scope.disableBufferUpdate == NULL)
        {
            goto error_handler_disableBufferUpdate;
        }
        sco->scope.historyUpdateCounter = (int *) CALLOC(block->nin, sizeof(int));
        if (sco->scope.historyUpdateCounter == NULL)
        {
            goto error_handler_historyUpdateCounter;
        }

        sco->scope.cachedFigureUID = 0;
        sco->scope.cachedAxeUID = (int*)CALLOC(block->nin, sizeof(int));

        sco->scope.cachedBufferPolylinesUIDs = (int**)CALLOC(block->nin, sizeof(int*));
        sco->scope.cachedHistoryPolylinesUIDs = (int**)CALLOC(block->nin, sizeof(int*));
        for (i = 0; i < block->nin; i++)
        {
            sco->scope.cachedBufferPolylinesUIDs[i] = (int*)CALLOC(block->insz[i], sizeof(int));
            sco->scope.cachedHistoryPolylinesUIDs[i] = (int*)CALLOC(block->insz[i], sizeof(int));
        }

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_historyUpdateCounter:
    FREE(sco->scope.disableBufferUpdate);
error_handler_disableBufferUpdate:
    FREE(sco->scope.periodCounter);
error_handler_periodCounter:
    i = block->nin;
error_handler_historyCoordinates_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.historyCoordinates[j]);
    }
    FREE(sco->internal.historyCoordinates);
error_handler_historyCoordinates:
error_handler_bufferCoordinates_ij:
    for (i = 0; i < block->nin - 1; i++)
    {
        for (j = 0; j < block->insz[i] - 1; j++)
        {
            double* ptr = sco->internal.bufferCoordinates[i][j];
            if (ptr != NULL)
            {
                FREE(ptr);
            }
        }
    }
    i = block->nin - 1;
error_handler_bufferCoordinates_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.bufferCoordinates[j]);
    }
    FREE(sco->internal.bufferCoordinates);
error_handler_bufferCoordinates:
    FREE(sco->internal.maxNumberOfPoints);
error_handler_maxNumberOfPoints:
    FREE(sco->internal.numberOfPoints);
error_handler_numberOfPoints:
    FREE(sco);
error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i, j;

    if (sco != NULL)
    {
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                if (sco->internal.historyCoordinates[i][j] != NULL)
                {
                    FREE(sco->internal.historyCoordinates[i][j]);
                }
                FREE(sco->internal.bufferCoordinates[i][j]);
            }
            FREE(sco->internal.historyCoordinates[i]);
            FREE(sco->internal.bufferCoordinates[i]);
        }
        FREE(sco->internal.historyCoordinates);
        FREE(sco->internal.bufferCoordinates);

        FREE(sco->scope.periodCounter);

        FREE(sco->scope.disableBufferUpdate);
        FREE(sco->scope.historyUpdateCounter);

        for (i = 0; i < block->nin; i++)
        {
            FREE(sco->scope.cachedHistoryPolylinesUIDs[i]);
            FREE(sco->scope.cachedBufferPolylinesUIDs[i]);
        }

        FREE(sco->scope.cachedHistoryPolylinesUIDs);
        FREE(sco->scope.cachedBufferPolylinesUIDs);
        FREE(sco->scope.cachedAxeUID);
        FREE(sco);
        *(block->work) = NULL;
    }
}

static sco_data *reallocHistoryBuffer(scicos_block * block, int input, int numberOfPoints)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i;

    double *ptr;
    int allocatedNumberOfPoints;

    int previousNumberOfPoints = sco->internal.maxNumberOfPoints[input];
    int numberOfCopiedPoints = numberOfPoints - sco->internal.maxNumberOfPoints[input];

    double *buffer;
    int bufferNumberOfPoints = block->ipar[2];
    int bufferNewPointInc;

    if (previousNumberOfPoints == 0)
    {
        allocatedNumberOfPoints = numberOfPoints;
        bufferNewPointInc = 0;
    }
    else
    {
        allocatedNumberOfPoints = numberOfPoints - 1;
        bufferNewPointInc = 1;
    }

    if (sco->scope.historyUpdateCounter[input] <= 0)
    {
        if (numberOfPoints > HISTORY_POINTS_THRESHOLD)
        {
            sco->scope.disableBufferUpdate[input] = TRUE;
            sco->scope.historyUpdateCounter[input] = numberOfPoints / HISTORY_POINTS_THRESHOLD;
        }
        else
        {
            sco->scope.disableBufferUpdate[input] = FALSE;
            sco->scope.historyUpdateCounter[input] = 0;
        }
    }

    for (i = 0; i < block->insz[input]; i++)
    {
        ptr = (double *)MALLOC(3 * allocatedNumberOfPoints * sizeof(double));
        if (ptr == NULL)
        {
            goto error_handler;
        }

        // memcpy existing X-axis values from the history
        memcpy(ptr, sco->internal.historyCoordinates[input][i], previousNumberOfPoints * sizeof(double));
        // memcpy existing Y-axis values from the history
        memcpy(ptr + allocatedNumberOfPoints, sco->internal.historyCoordinates[input][i] + previousNumberOfPoints, previousNumberOfPoints * sizeof(double));
        // clear the last points, the Z-axis values
        memset(ptr + 2 * allocatedNumberOfPoints, 0, allocatedNumberOfPoints * sizeof(double));

        // then set the last points to the last values for X-axis and Y-axis values from the buffer points
        buffer = sco->internal.bufferCoordinates[input][i];
        memcpy(ptr + previousNumberOfPoints, buffer + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));
        memcpy(ptr + allocatedNumberOfPoints + previousNumberOfPoints, buffer + bufferNumberOfPoints + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));

        FREE(sco->internal.historyCoordinates[input][i]);
        sco->internal.historyCoordinates[input][i] = ptr;
    }

    sco->internal.maxNumberOfPoints[input] = allocatedNumberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void setBuffersCoordinates(scicos_block * block, int input, double* coordinates, const int numberOfPoints,
                                  const int bufferPoints, const double t, const double value)
{
    int setLen;
    sco_data *sco = (sco_data *) * (block->work);

    if (sco->scope.disableBufferUpdate[input] == TRUE)
    {
        coordinates[numberOfPoints] = t;
        coordinates[bufferPoints + numberOfPoints] = value;
        return;
    }

    // X-axis values first
    for (setLen = numberOfPoints; setLen < bufferPoints; setLen++)
    {
        coordinates[setLen] = t;
    }
    // then Y-axis values
    for (setLen = numberOfPoints; setLen < bufferPoints; setLen++)
    {
        coordinates[bufferPoints + setLen] = value;
    }
    // then Z-axis values (always clear'ed)
}

static void appendData(scicos_block * block, int input, double t, double *data)
{
    int i;

    sco_data *sco = (sco_data *) * (block->work);

    /*
     * Handle the case where the t is greater than the data_bounds
     */
    if (t > ((sco->scope.periodCounter[input] + 1) * block->rpar[1 + input]))
    {
        sco->scope.periodCounter[input]++;

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][sco->internal.numberOfPoints[input] - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][block->ipar[2] + sco->internal.numberOfPoints[input] - 1];
        }
        sco->internal.numberOfPoints[input] = 1;

        // clear the history coordinates
        sco->internal.maxNumberOfPoints[input] = 0;
        for (i = 0; i < block->insz[input]; i++)
        {
            if (sco->internal.historyCoordinates[input][i] != NULL)
            {
                FREE(sco->internal.historyCoordinates[input][i]);
                sco->internal.historyCoordinates[input][i] = NULL;
            }
        }

        // configure scope setting
        if (setPolylinesBounds(block, getAxe(getFigure(block), block, input), input, sco->scope.periodCounter[input]) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && sco->internal.numberOfPoints[input] >= block->ipar[2])
    {
        int maxNumberOfPoints = sco->internal.maxNumberOfPoints[input];

        // on a full scope, re-alloc history coordinates
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocHistoryBuffer(block, input, maxNumberOfPoints);

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][block->ipar[2] - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][2 * block->ipar[2] - 1];
        }
        sco->internal.numberOfPoints[input] = 1;

        // reconfigure related graphic objects
        if (pushHistory(block, input, sco->internal.maxNumberOfPoints[input]) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Update data
     */
    if (sco != NULL)
    {
        for (i = 0; i < block->insz[input]; i++)
        {
            const double value = data[i];
            setBuffersCoordinates(block, input, sco->internal.bufferCoordinates[input][i], sco->internal.numberOfPoints[input], block->ipar[2], t, value);
        }

        sco->internal.numberOfPoints[input]++;
    }
}

static BOOL pushData(scicos_block * block, int input, int row)
{
    int iFigureUID;
    int iAxeUID;
    int iPolylineUID;

    double *data;
    sco_data *sco;

    iFigureUID = getFigure(block);
    iAxeUID = getAxe(iFigureUID, block, input);
    iPolylineUID = getPolyline(iAxeUID, block, input, row, FALSE);

    sco = getScoData(block);
    if (sco == NULL)
    {
        return FALSE;
    }

    // do not push any data if disabled
    if (sco->scope.disableBufferUpdate[input] == TRUE)
    {
        return TRUE;
    }

    // select the right input and row
    data = sco->internal.bufferCoordinates[input][row];

    PUSH_LOG("%s: %d\n", "pushData", block->ipar[2]);
    return setGraphicObjectProperty(iPolylineUID, __GO_DATA_MODEL_COORDINATES__, data, jni_double_vector, block->ipar[2]);
}

/*****************************************************************************
 *
 * Graphic utils
 *
 ****************************************************************************/

/**
 * Set properties on the figure.
 *
 * \param iFigureUID the figure uid
 * \param block the current block
 */
static void setFigureSettings(int iFigureUID, scicos_block * block)
{
    int win_pos[2];
    int win_dim[2];
    char* label;

    int *ipar = block->ipar;

    win_pos[0] = ipar[3];
    win_pos[1] = ipar[4];
    win_dim[0] = ipar[5];
    win_dim[1] = ipar[6];

    if (win_pos[0] > 0 && win_pos[1] > 0)
    {
        setGraphicObjectProperty(iFigureUID, __GO_POSITION__, &win_pos, jni_int_vector, 2);
    }

    if (win_dim[0] > 0 && win_dim[1] > 0)
    {
        setGraphicObjectProperty(iFigureUID, __GO_SIZE__, &win_dim, jni_int_vector, 2);
    }

    label = GetLabelPtrs(block);
    if (label != NULL)
    {
        if (strlen(label) > 0)
        {
            setGraphicObjectProperty(iFigureUID, __GO_NAME__, label, jni_string, 1);
        }
    }
}

/**
 * Set properties on the axes.
 *
 * \param iAxeUID the axe uid
 * \param block the current block
 * \param index axe index (0-indexed)
 */
static void setAxesSettings(int iAxeUID, scicos_block * block, int index)
{
    double axesBounds[4];
    double margins[4];

    double nin = (double) block->nin;

    axesBounds[0] = 0;              // x
    axesBounds[1] = index / nin;    // y
    axesBounds[2] = 1.0;            // w
    axesBounds[3] = 1 / nin;        // h
    setGraphicObjectProperty(iAxeUID, __GO_AXES_BOUNDS__, axesBounds, jni_double_vector, 4);

    margins[0] = 0.125;
    margins[1] = 0.125;
    margins[2] = 0.125;
    margins[3] = 0.125;
    setGraphicObjectProperty(iAxeUID, __GO_MARGINS__, margins, jni_double_vector, 4);

};

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
    sco_data *sco = (sco_data *) * (block->work);

    int i;

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

        // set configured parameters
        setFigureSettings(iFigureUID, block);

        // allocate the axes through the getter
        for (i = 0; i < GetNin(block); i++)
        {
            iAxe = getAxe(iFigureUID, block, i);

            /*
             * Setup according to block settings
             */
            setLabel(iAxe, __GO_X_AXIS_LABEL__, "t");
            setLabel(iAxe, __GO_Y_AXIS_LABEL__, "y");

            setGraphicObjectProperty(iAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
            setGraphicObjectProperty(iAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);

            setPolylinesBounds(block, iAxe, i, 0);
        }
    }
    else
    {
        // set configured parameters
        setFigureSettings(iFigureUID, block);
    }

    if (sco->scope.cachedFigureUID == 0)
    {
        sco->scope.cachedFigureUID = iFigureUID;
    }
    return iFigureUID;
}

static int getAxe(int iFigureUID, scicos_block * block, int input)
{
    int iAxe;
    int i;
    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL || sco->scope.cachedAxeUID == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (sco->scope.cachedAxeUID[input])
    {
        return sco->scope.cachedAxeUID[input];
    }

    iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, input);

    /*
     * Allocate if necessary
     */
    if (iAxe == 0)
    {
        cloneAxesModel(iFigureUID);
        iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, input);

        // seems that the graphic is unable to clone the default axe model
        if (iAxe == 0)
        {
            return 0;
        }
    }

    /*
     * Setup on first access
     */
    // allocate the polylines through the getter
    for (i = 0; i < block->insz[input]; i++)
    {
        getPolyline(iAxe, block, input, i, TRUE);
    }
    for (i = 0; i < block->insz[input]; i++)
    {
        getPolyline(iAxe, block, input, i, FALSE);
    }

    setAxesSettings(iAxe, block, input);

    /*
     * then cache with local storage
     */
    sco->scope.cachedAxeUID[input] = iAxe;
    return sco->scope.cachedAxeUID[input];
}

static int getPolyline(int iAxeUID, scicos_block * block, int input, int row, BOOL history)
{
    int iPolyline;
    BOOL b__true = TRUE;

    int color;

    int** piPolylinesUIDs;
    int polylineIndex;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }

    if (!history)
    {
        piPolylinesUIDs = sco->scope.cachedBufferPolylinesUIDs;
        polylineIndex = block->insz[input] + row;
    }
    else
    {
        piPolylinesUIDs = sco->scope.cachedHistoryPolylinesUIDs;
        polylineIndex = row;
    }

    // assert that the structure is in a good shape
    if (piPolylinesUIDs == NULL || piPolylinesUIDs[input] == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (piPolylinesUIDs[input][row] != 0)
    {
        return piPolylinesUIDs[input][row];
    }

    iPolyline = findChildWithKindAt(iAxeUID, __GO_POLYLINE__, polylineIndex);

    /*
     * Allocate if necessary
     */
    if (iPolyline == 0)
    {
        iPolyline = createGraphicObject(__GO_POLYLINE__);

        if (iPolyline != 0)
        {
            createDataObject(iPolyline, __GO_POLYLINE__);
            setGraphicObjectRelationship(iAxeUID, iPolyline);
        }
        else
        {
            return 0;
        }
    }

    /*
     * Setup on first access
     */

    /*
     * Default setup of the nGons property
     */
    {
        int nGons = 1;
        setGraphicObjectProperty(iPolyline, __GO_DATA_MODEL_NUM_GONS__, &nGons, jni_int, 1);
    }

    // ipar=[win;size(in,'*');N;wpos(:);wdim(:);in(:);clrs(:);heritance]
    //        1     1         1   2       2      nin   nin       1
    color = block->ipar[7 + block->nin + input + row];
    if (color > 0)
    {
        LOG("%s: %s at %d at %d to %d\n", "cmscope", "set lines mode", input, row, color);

        setGraphicObjectProperty(iPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);
        setGraphicObjectProperty(iPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
    }
    else
    {
        int iMarkSize = 4;
        color = -color;

        LOG("%s: %s at %d at %d to %d\n", "cmscope", "set mark mode", input, row, -color);

        setGraphicObjectProperty(iPolyline, __GO_MARK_MODE__, &b__true, jni_bool, 1);
        setGraphicObjectProperty(iPolyline, __GO_MARK_STYLE__, &color, jni_int, 1);
        setGraphicObjectProperty(iPolyline, __GO_MARK_SIZE__, &iMarkSize, jni_int, 1);
    }

    {
        int iClipState = 1; //on
        setGraphicObjectProperty(iPolyline, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
    }

    /*
     * then cache with local storage
     */
    piPolylinesUIDs[input][row] = iPolyline;
    return piPolylinesUIDs[input][row];
}

static void deleteBufferPolylines(scicos_block * block)
{
    int i, j;

    int iPolylineUID;

    sco_data *sco;

    sco = getScoData(block);
    for (i = 0; i < block->nin; i++)
    {
        for (j = 0; j < block->insz[i]; j++)
        {
            iPolylineUID = sco->scope.cachedBufferPolylinesUIDs[i][j];
            deleteGraphicObject(iPolylineUID);
        }
    }
}

static BOOL pushHistory(scicos_block * block, int input, int maxNumberOfPoints)
{
    int i;

    int iFigureUID;
    int iAxeUID;
    int iPolylineUID;

    double *data;
    sco_data *sco;

    BOOL result = TRUE;

    sco = getScoData(block);
    iFigureUID = getFigure(block);
    iAxeUID = getAxe(iFigureUID, block, input);

    // push the data only if the counter == 0, decrement the counter if positive
    if (sco->scope.historyUpdateCounter[input] > 0)
    {
        sco->scope.historyUpdateCounter[input]--;
    }
    if (sco->scope.historyUpdateCounter[input] > 0)
    {
        return result;
    }

    for (i = 0; i < block->insz[input]; i++)
    {
        iPolylineUID = getPolyline(iAxeUID, block, input, i, TRUE);

        data = sco->internal.historyCoordinates[input][i];

        PUSH_LOG("%s: %d\n", "pushHistory", maxNumberOfPoints);
        result = setGraphicObjectProperty(iPolylineUID, __GO_DATA_MODEL_COORDINATES__, data, jni_double_vector, maxNumberOfPoints);
        if (result == FALSE)
        {
            return result;
        }
    }

    return result;
}

static BOOL setPolylinesBounds(scicos_block * block, int iAxeUID, int input, int periodCounter)
{
    double dataBounds[6];
    int nin = block->nin;
    double period = block->rpar[block->nrpar - 3 * nin + input];

    dataBounds[0] = periodCounter * period; // xMin
    dataBounds[1] = (periodCounter + 1) * period;   // xMax
    dataBounds[2] = block->rpar[block->nrpar - 2 * nin + 2 * input];    // yMin
    dataBounds[3] = block->rpar[block->nrpar - 2 * nin + 2 * input + 1];    // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    LOG("%s: %s at %d to %f\n", "cmscope", "setPolylinesBounds", input, dataBounds[1]);

    return setGraphicObjectProperty(iAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
