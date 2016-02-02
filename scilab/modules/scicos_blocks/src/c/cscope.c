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
        int numberOfPoints;
        double ***bufferCoordinates;
        int maxNumberOfPoints;
        double ***historyCoordinates;
    } internal;

    struct
    {
        int periodCounter;

        BOOL disableBufferUpdate;
        int historyUpdateCounter;

        int cachedFigureUID;
        int cachedAxeUID;
        int* cachedBufferPolylinesUIDs;
        int* cachedHistoryPolylinesUIDs;
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
 * \param numberOfPoints realloc to store this number of points
 */
static sco_data *reallocHistoryBuffer(scicos_block * block, int numberOfPoints);

/**
 * Set values into the coordinates buffer.
 *
 * \param block the block
 * \param coordinates the buffer
 * \param numberOfPoints the number of points to set (actual)
 * \param bufferPoints the buffer size (max)
 * \param t the time to set
 * \param value the value to set
 */
static void setBuffersCoordinates(scicos_block * block, double* coordinates, const int numberOfPoints,
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
 * \param row the current row index (0-indexed)
 * \param history get the history polyline
 * \return a valid polyline UID or NULL on error
 */
static int getPolyline(int iAxeUID, scicos_block * block, int row, BOOL history);

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
 * \param block the current block
 * \param iAxeUID the axe uid
 * \param periodCounter number of past periods since startup
 */
static BOOL setPolylinesBounds(scicos_block * block, int iAxeUID, int periodCounter);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cscope(scicos_block * block, scicos_flag flag)
{
    int iFigureUID;

    double t;
    double *u;
    sco_data *sco;

    int i;
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
            u = GetRealInPortPtrs(block, 1);

            appendData(block, 0, t, u);

            for (i = 0; i < block->insz[0]; i++)
            {
                result = pushData(block, 0, i);
                if (result == FALSE)
                {
                    Coserror("%s: unable to push some data.", "cscope");
                    break;
                }
            }
            break;

        case Ending:
            sco = getScoData(block);
            sco = reallocHistoryBuffer(block, sco->internal.maxNumberOfPoints + sco->internal.numberOfPoints);
            sco->scope.disableBufferUpdate = FALSE;
            sco->scope.historyUpdateCounter = 0;
#ifdef WITH_GUI
            pushHistory(block, 0, sco->internal.maxNumberOfPoints);
            deleteBufferPolylines(block);
#endif
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

        // 0 points out of a block->ipar[2] points buffer
        sco->internal.numberOfPoints = 0;

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

        // 0 points out of a 0 points history
        sco->internal.maxNumberOfPoints = 0;

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

        sco->scope.periodCounter = 0;

        // flag to avoid pushing the buffer each time
        sco->scope.disableBufferUpdate = FALSE;
        // counter use to delay the history push
        sco->scope.historyUpdateCounter = 0;

        sco->scope.cachedFigureUID = 0;
        sco->scope.cachedAxeUID = 0;
        sco->scope.cachedBufferPolylinesUIDs = (int*)CALLOC(block->insz[0], sizeof(int));
        sco->scope.cachedHistoryPolylinesUIDs = (int*)CALLOC(block->insz[0], sizeof(int));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

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

        FREE(sco->scope.cachedHistoryPolylinesUIDs);
        FREE(sco->scope.cachedBufferPolylinesUIDs);

        FREE(sco);
        *(block->work) = NULL;
    }
}

static sco_data *reallocHistoryBuffer(scicos_block * block, int numberOfPoints)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i;

    double *ptr;
    int allocatedNumberOfPoints;

    int previousNumberOfPoints = sco->internal.maxNumberOfPoints;
    int numberOfCopiedPoints = numberOfPoints - sco->internal.maxNumberOfPoints;

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

    if (sco->scope.historyUpdateCounter <= 0)
    {
        if (numberOfPoints > HISTORY_POINTS_THRESHOLD)
        {
            sco->scope.disableBufferUpdate = TRUE;
            sco->scope.historyUpdateCounter = numberOfPoints / HISTORY_POINTS_THRESHOLD;
        }
        else
        {
            sco->scope.disableBufferUpdate = FALSE;
            sco->scope.historyUpdateCounter = 0;
        }
    }

    for (i = 0; i < block->insz[0]; i++)
    {
        ptr = (double *)MALLOC(3 * allocatedNumberOfPoints * sizeof(double));
        if (ptr == NULL)
        {
            goto error_handler;
        }

        // memcpy existing X-axis values from the history
        memcpy(ptr, sco->internal.historyCoordinates[0][i], previousNumberOfPoints * sizeof(double));
        // memcpy existing Y-axis values from the history
        memcpy(ptr + allocatedNumberOfPoints, sco->internal.historyCoordinates[0][i] + previousNumberOfPoints, previousNumberOfPoints * sizeof(double));
        // clear the last points, the Z-axis values
        memset(ptr + 2 * allocatedNumberOfPoints, 0, allocatedNumberOfPoints * sizeof(double));

        // then set the last points to the last values for X-axis and Y-axis values from the buffer points
        buffer = sco->internal.bufferCoordinates[0][i];
        memcpy(ptr + previousNumberOfPoints, buffer + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));
        memcpy(ptr + allocatedNumberOfPoints + previousNumberOfPoints, buffer + bufferNumberOfPoints + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));

        FREE(sco->internal.historyCoordinates[0][i]);
        sco->internal.historyCoordinates[0][i] = ptr;
    }

    sco->internal.maxNumberOfPoints = allocatedNumberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void setBuffersCoordinates(scicos_block* block, double* coordinates, const int numberOfPoints,
                                  const int bufferPoints, const double t, const double value)
{
    int setLen;
    sco_data *sco = (sco_data *) * (block->work);

    if (sco->scope.disableBufferUpdate == TRUE)
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
    if (t > ((sco->scope.periodCounter + 1) * block->rpar[3]))
    {
        sco->scope.periodCounter++;

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][sco->internal.numberOfPoints - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][block->ipar[2] + sco->internal.numberOfPoints - 1];
        }
        sco->internal.numberOfPoints = 1;

        // clear the history coordinates
        sco->internal.maxNumberOfPoints = 0;
        for (i = 0; i < block->insz[input]; i++)
        {
            if (sco->internal.historyCoordinates[input][i] != NULL)
            {
                FREE(sco->internal.historyCoordinates[input][i]);
                sco->internal.historyCoordinates[input][i] = NULL;
            }
        }

        // configure scope setting
        if (setPolylinesBounds(block, getAxe(getFigure(block), block, input), sco->scope.periodCounter) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && sco->internal.numberOfPoints >= block->ipar[2])
    {
        int maxNumberOfPoints = sco->internal.maxNumberOfPoints;

        // on a full scope, re-alloc history coordinates
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocHistoryBuffer(block, maxNumberOfPoints);

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][block->ipar[2] - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][2 * block->ipar[2] - 1];
        }
        sco->internal.numberOfPoints = 1;

        // reconfigure related graphic objects
        if (pushHistory(block, input, sco->internal.maxNumberOfPoints) == FALSE)
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
            setBuffersCoordinates(block, sco->internal.bufferCoordinates[input][i], sco->internal.numberOfPoints, block->ipar[2], t, value);
        }

        sco->internal.numberOfPoints++;
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
    iPolylineUID = getPolyline(iAxeUID, block, row, FALSE);

    sco = getScoData(block);
    if (sco == NULL)
    {
        return FALSE;
    }

    // do not push any data if disabled
    if (sco->scope.disableBufferUpdate == TRUE)
    {
        return TRUE;
    }

    // select the right input and row
    data = sco->internal.bufferCoordinates[input][row];

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
    char *label = NULL;

    int nipar = GetNipar(block);
    int *ipar = GetIparPtrs(block);

    int win_pos[2];
    int win_dim[2];

    win_pos[0] = ipar[(nipar - 1) - 3];
    win_pos[1] = ipar[(nipar - 1) - 2];
    win_dim[0] = ipar[(nipar - 1) - 1];
    win_dim[1] = ipar[nipar - 1];

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
};

/*****************************************************************************
 *
 * Graphic
 *
 ****************************************************************************/

static int getFigure(scicos_block * block)
{
#ifdef WITH_GUI
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
    if (sco->scope.cachedFigureUID != 0)
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

            setPolylinesBounds(block, iAxe, 0);
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
#else
    Coserror("%s: Scilab is compiled without GUI, can not use Scope.", "cscope");
    return 0;
#endif
}

static int getAxe(int iFigureUID, scicos_block * block, int input)
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
    if (sco->scope.cachedAxeUID != 0)
    {
        return sco->scope.cachedAxeUID;
    }

    iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, input);

    /*
     * Allocate if necessary
     */
    if (iAxe == 0)
    {
        cloneAxesModel(iFigureUID);
        iAxe = findChildWithKindAt(iFigureUID, __GO_AXES__, input);
    }

    /*
     * Setup on first access
     */
    if (iAxe != 0)
    {
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[input]; i++)
        {
            getPolyline(iAxe, block, i, TRUE);
        }
        for (i = 0; i < block->insz[input]; i++)
        {
            getPolyline(iAxe, block, i, FALSE);
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

static int getPolyline(int iAxeUID, scicos_block * block, int row, BOOL history)
{
    int iPolyline;
    BOOL b__true = TRUE;

    int color;

    int* polylinesUIDs;
    int polylineIndex;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return 0;
    }

    if (!history)
    {
        polylinesUIDs = sco->scope.cachedBufferPolylinesUIDs;
        polylineIndex = block->insz[0] + row;
    }
    else
    {
        polylinesUIDs = sco->scope.cachedHistoryPolylinesUIDs;
        polylineIndex = row;
    }

    // assert that the structure is in a good shape
    if (polylinesUIDs == NULL)
    {
        return 0;
    }

    // fast path for an existing object
    if (polylinesUIDs[row] != 0)
    {
        return polylinesUIDs[row];
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

    color = block->ipar[3 + row];
    if (color > 0)
    {
        setGraphicObjectProperty(iPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);
        setGraphicObjectProperty(iPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
    }
    else
    {
        int iMarkSize = 4;
        color = -color;
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
    polylinesUIDs[row] = iPolyline;
    return polylinesUIDs[row];
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
            iPolylineUID = sco->scope.cachedBufferPolylinesUIDs[j];
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
    if (sco->scope.historyUpdateCounter > 0)
    {
        sco->scope.historyUpdateCounter--;
    }
    if (sco->scope.historyUpdateCounter > 0)
    {
        return result;
    }

    for (i = 0; i < block->insz[input]; i++)
    {
        iPolylineUID = getPolyline(iAxeUID, block, i, TRUE);

        data = sco->internal.historyCoordinates[input][i];
        result = setGraphicObjectProperty(iPolylineUID, __GO_DATA_MODEL_COORDINATES__, data, jni_double_vector, maxNumberOfPoints);
        if (result == FALSE)
        {
            return result;
        }
    }

    return result;
}

static BOOL setPolylinesBounds(scicos_block * block, int iAxeUID, int periodCounter)
{
    double dataBounds[6];
    double period = block->rpar[3];

    dataBounds[0] = periodCounter * period; // xMin
    dataBounds[1] = (periodCounter + 1) * period;   // xMax
    dataBounds[2] = block->rpar[1]; // yMin
    dataBounds[3] = block->rpar[2]; // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    return setGraphicObjectProperty(iAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
