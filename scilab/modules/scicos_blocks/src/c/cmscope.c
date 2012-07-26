/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "MALLOC.h"
#include "elementary_functions.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"
#include "CurrentObject.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "FigureList.h"
#include "BuildObjects.h"
#include "AxesModel.h"

// #include <stdio.h>
// #define LOG(...) printf(__VA_ARGS__)
#define LOG(...)

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
        int *maxNumberOfPoints;
        double **time;
        double ***data;
    } internal;

    struct
    {
        int *periodCounter;

        char const* cachedFigureUID;
        char **cachedAxeUID;
        char ***cachedPolylinesUIDs;
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
 * Realloc any internal data
 *
 * \param block the block
 * \param input the selected input
 * \param numberOfPoints realloc to store this number of points
 */
static sco_data *reallocScoData(scicos_block * block, int input, int numberOfPoints);

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
 * \param pPolylineUID the polyline uid
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
static char const* getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param pFigureUID the parent figure UID
 * \param block the block
 * \param input the current input index (0-indexed)
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char const* pFigureUID, scicos_block * block, int input);

/**
 * Get (and allocate on demand) the polyline associated with the row
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \param input the current input index (0-indexed)
 * \param row the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static char *getPolyline(char *pAxeUID, scicos_block * block, int input, int row);

/**
 * Set the polylines buffer size
 *
 * \param block the block
 * \param input the input port index
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL setPolylinesBuffers(scicos_block * block, int input, int maxNumberOfPoints);

/**
 * Set the polylines bounds
 *
 * \param block the block
 * \param input the input port index
 * \param periodCounter number of past periods since startup
 */
static BOOL setPolylinesBounds(scicos_block * block, int input, int periodCounter);

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
    char const* pFigureUID;

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
    int i, j, k, l;

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
            goto error_handler_sco;

        sco->internal.numberOfPoints = (int *) MALLOC(block->nin * sizeof(int));
        if (sco->internal.numberOfPoints == NULL)
            goto error_handler_numberOfPoints;
        sco->internal.maxNumberOfPoints = (int *) MALLOC(block->nin * sizeof(int));
        if (sco->internal.maxNumberOfPoints == NULL)
            goto error_handler_maxNumberOfPoints;

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.numberOfPoints[i] = 0;
            sco->internal.maxNumberOfPoints[i] = block->ipar[2];
        }

        sco->internal.data = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.data == NULL)
            goto error_handler_data;

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.data[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.data[i] == NULL)
                goto error_handler_data_i;
        }
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                sco->internal.data[i][j] = (double *)CALLOC(block->ipar[2], sizeof(double));

                if (sco->internal.data[i][j] == NULL)
                    goto error_handler_data_ij;
            }
        }

        sco->internal.time = (double **) MALLOC(block->nin * sizeof(double));
        if (sco->internal.time == NULL)
        {
            goto error_handler_time;
        }
        for (i = 0; i < block->nin; i++)
        {
            sco->internal.time[i] = (double *)CALLOC(block->ipar[2], sizeof(double));
            if (sco->internal.time[i] == NULL)
            {
                goto error_handler_time_i;
            }
        }

        sco->scope.periodCounter = (int *) CALLOC(block->nin, sizeof(int));
        if (sco->scope.periodCounter == NULL)
            goto error_handler_periodCounter;

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = (char **)CALLOC(block->nin, sizeof(char *));

        sco->scope.cachedPolylinesUIDs = (char ***)CALLOC(block->nin, sizeof(char **));
        for (i = 0; i < block->nin; i++)
        {
            sco->scope.cachedPolylinesUIDs[i] = (char **)CALLOC(block->insz[i], sizeof(char *));
        }

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */
error_handler_periodCounter:
error_handler_time_i:
    for (k = 0; k < i; k++)
    {
        FREE(sco->internal.time[i]);
    }
    i = block->nin - 1;
    j = 2 ^ 31;
    FREE(sco->internal.time);
error_handler_time:
error_handler_data_ij:
    for (k = 0; k < i; k++)
    {
        for (l = 0; l < j && l < block->insz[i]; l++)
        {
            FREE(sco->internal.data[k][l]);
        }
    }
    i = block->nin - 1;
error_handler_data_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.data[i]);
    }
    FREE(sco->internal.data);
error_handler_data:
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
                FREE(sco->internal.data[i][j]);
            }
            FREE(sco->internal.data[i]);
        }
        FREE(sco->internal.data);

        for (i = 0; i < block->nin; i++)
        {
            FREE(sco->internal.time[i]);
        }
        FREE(sco->internal.time);

        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                FREE(sco->scope.cachedPolylinesUIDs[i][j]);
            }
            FREE(sco->scope.cachedAxeUID[i]);
        }

        FREE(sco);
        *(block->work) = NULL;
    }
}

static sco_data *reallocScoData(scicos_block * block, int input, int numberOfPoints)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i, j;

    double *ptr;
    int setLen;
    int previousNumberOfPoints = sco->internal.maxNumberOfPoints[input];

    LOG("%s: %s at %d to %d\n", "cmscope", "reallocScoData", input, numberOfPoints);

    for (i = 0; i < block->nin; i++)
    {
        for (j = 0; j < block->insz[i]; j++)
        {
            ptr = (double *)REALLOC(sco->internal.data[i][j], numberOfPoints * sizeof(double));
            if (ptr == NULL)
                goto error_handler;

            for (setLen = numberOfPoints - previousNumberOfPoints - 1; setLen >= 0; setLen--)
            {
                ptr[previousNumberOfPoints + setLen] = ptr[previousNumberOfPoints - 1];
            }
            sco->internal.data[i][j] = ptr;
        }
    }

    ptr = (double *)REALLOC(sco->internal.time[input], numberOfPoints * sizeof(double));
    if (ptr == NULL)
        goto error_handler;

    for (setLen = numberOfPoints - previousNumberOfPoints - 1; setLen >= 0; setLen--)
    {
        ptr[previousNumberOfPoints + setLen] = ptr[previousNumberOfPoints - 1];
    }
    sco->internal.time[input] = ptr;

    sco->internal.maxNumberOfPoints[input] = numberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void appendData(scicos_block * block, int input, double t, double *data)
{
    int i;

    sco_data *sco = (sco_data *) * (block->work);
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints[input];
    int numberOfPoints = sco->internal.numberOfPoints[input];


    /*
     * Handle the case where the t is greater than the data_bounds
     */
    if (t > ((sco->scope.periodCounter[input] + 1) * block->rpar[1 + input]))
    {
        sco->scope.periodCounter[input]++;

        numberOfPoints = 0;
        sco->internal.numberOfPoints[input] = 0;
        if (setPolylinesBounds(block, input, sco->scope.periodCounter[input]) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && numberOfPoints >= maxNumberOfPoints)
    {
        // on a full scope, re-alloc
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocScoData(block, input, maxNumberOfPoints);

        // reconfigure related graphic objects
        if (setPolylinesBuffers(block, input, maxNumberOfPoints) == FALSE)
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
        int setLen;

        for (i = 0; i < block->insz[input]; i++)
        {
            for (setLen = maxNumberOfPoints - numberOfPoints - 1; setLen >= 0; setLen--)
            {
                sco->internal.data[input][i][numberOfPoints + setLen] = data[i];
            }
        }

        for (setLen = maxNumberOfPoints - numberOfPoints - 1; setLen >= 0; setLen--)
        {
            sco->internal.time[input][numberOfPoints + setLen] = t;
        }

        sco->internal.numberOfPoints[input]++;
    }
}

static BOOL pushData(scicos_block * block, int input, int row)
{
    char const* pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    double *data;
    sco_data *sco;

    BOOL result = TRUE;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);
    pPolylineUID = getPolyline(pAxeUID, block, input, row);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;


    // select the right input and row
    data = sco->internal.data[input][row];

    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_X__, sco->internal.time[input], jni_double_vector, sco->internal.maxNumberOfPoints[input]);
    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_Y__, data, jni_double_vector, sco->internal.maxNumberOfPoints[input]);

    return result;
}

/*****************************************************************************
 *
 * Graphic utils
 *
 ****************************************************************************/

/**
 * Set properties on the figure.
 *
 * \param pFigureUID the figure uid
 * \param block the current block
 */
static void setFigureSettings(char const* pFigureUID, scicos_block * block)
{
    int win_pos[2];
    int win_dim[2];

    int *ipar = block->ipar;

    win_pos[0] = ipar[3];
    win_pos[1] = ipar[4];
    win_dim[0] = ipar[5];
    win_dim[1] = ipar[6];

    if (win_pos[0] > 0 && win_pos[1] > 0)
    {
        setGraphicObjectProperty(pFigureUID, __GO_POSITION__, &win_pos, jni_int_vector, 2);
    }

    if (win_dim[0] > 0 && win_dim[1] > 0)
    {
        setGraphicObjectProperty(pFigureUID, __GO_SIZE__, &win_dim, jni_int_vector, 2);
    }
};

/**
 * Set properties on the axes.
 *
 * \param pAxeUID the axe uid
 * \param block the current block
 * \param index axe index (0-indexed)
 */
static void setAxesSettings(char *pAxeUID, scicos_block * block, int index)
{
    double axesBounds[4];
    double margins[4];

    double nin = (double) block->nin;

    axesBounds[0] = 0;              // x
    axesBounds[1] = index / nin;    // y
    axesBounds[2] = 1.0;            // w
    axesBounds[3] = 1 / nin;        // h
    setGraphicObjectProperty(pAxeUID, __GO_AXES_BOUNDS__, axesBounds, jni_double_vector, 4);

    margins[0] = 0.125;
    margins[1] = 0.125;
    margins[2] = 0.125;
    margins[3] = 0.125;
    setGraphicObjectProperty(pAxeUID, __GO_MARGINS__, margins, jni_double_vector, 4);

};

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

    int i;

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

        // set configured parameters
        setFigureSettings(pFigureUID, block);

        // allocate the axes through the getter
        for (i = 0; i < GetNin(block); i++)
        {
            pAxe = getAxe(pFigureUID, block, i);

            /*
             * Setup according to block settings
             */
            setLabel(pAxe, __GO_X_AXIS_LABEL__, "t");
            setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");

            setGraphicObjectProperty(pAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
            setGraphicObjectProperty(pAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);

            setPolylinesBounds(block, i, 0);
        }
    }

    if (sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char const* pFigureUID, scicos_block * block, int input)
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
    if (sco->scope.cachedAxeUID != NULL && sco->scope.cachedAxeUID[input] != NULL)
    {
        return sco->scope.cachedAxeUID[input];
    }

    pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, input);

    /*
     * Allocate if necessary
     */
    if (pAxe == NULL)
    {
        cloneAxesModel(pFigureUID);
        pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, input);
    }

    /*
     * Setup on first access
     */
    if (pAxe != NULL)
    {
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[input]; i++)
        {
            getPolyline(pAxe, block, input, i);
        }

        setAxesSettings(pAxe, block, input);
    }

    /*
     * then cache with local storage
     */
    if (pAxe != NULL && sco->scope.cachedAxeUID != NULL && sco->scope.cachedAxeUID[input] == NULL)
    {
        sco->scope.cachedAxeUID[input] = strdup(pAxe);
        releaseGraphicObjectProperty(__GO_PARENT__, pAxe, jni_string, 1);
    }
    return sco->scope.cachedAxeUID[input];
}

static char *getPolyline(char *pAxeUID, scicos_block * block, int input, int row)
{
    char *pPolyline;
    double d__0 = 0.0;
    BOOL b__true = TRUE;

    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (sco->scope.cachedPolylinesUIDs != NULL && sco->scope.cachedPolylinesUIDs[input] != NULL && sco->scope.cachedPolylinesUIDs[input][row] != NULL)
    {
        return sco->scope.cachedPolylinesUIDs[input][row];
    }

    pPolyline = findChildWithKindAt(pAxeUID, __GO_POLYLINE__, row);

    /*
     * Allocate if necessary
     */
    if (pPolyline == NULL)
    {
        pPolyline = createGraphicObject(__GO_POLYLINE__);

        if (pPolyline != NULL)
        {
            createDataObject(pPolyline, __GO_POLYLINE__);
            setGraphicObjectRelationship(pAxeUID, pPolyline);
        }
    }

    /*
     * Setup on first access
     */
    if (pPolyline != NULL)
    {

        /*
         * Default setup (will crash if removed)
         */
        {
            int polylineSize[2] = { 1, block->ipar[2] };
            setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
        }

        setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_X__, &d__0, jni_double_vector, 1);
        setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_Y__, &d__0, jni_double_vector, 1);

        // ipar=[win;size(in,'*');N;wpos(:);wdim(:);in(:);clrs(:);heritance]
        //        1     1         1   2       2      nin   nin       1
        color = block->ipar[7 + block->nin + input + row];
        if (color > 0)
        {
            LOG("%s: %s at %d at %d to %d\n", "cmscope", "set lines mode", input, row, color);

            setGraphicObjectProperty(pPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);
            setGraphicObjectProperty(pPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
        }
        else
        {
            color = -color;

            LOG("%s: %s at %d at %d to %d\n", "cmscope", "set mark mode", input, row, -color);

            setGraphicObjectProperty(pPolyline, __GO_MARK_MODE__, &b__true, jni_bool, 1);
            setGraphicObjectProperty(pPolyline, __GO_MARK_STYLE__, &color, jni_int, 1);
        }

        {
            int iClipState = 1; //on
            setGraphicObjectProperty(pPolyline, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
        }
    }

    /*
     * then cache with local storage
     */
    if (sco->scope.cachedPolylinesUIDs != NULL && sco->scope.cachedPolylinesUIDs[input] != NULL)
    {
        sco->scope.cachedPolylinesUIDs[input][row] = strdup(pPolyline);
        releaseGraphicObjectProperty(__GO_PARENT__, pPolyline, jni_string, 1);
    }
    return sco->scope.cachedPolylinesUIDs[input][row];
}

static BOOL setPolylinesBuffers(scicos_block * block, int input, int maxNumberOfPoints)
{
    int i;

    char const* pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    BOOL result = TRUE;
    int polylineSize[2] = { 1, maxNumberOfPoints };

    LOG("%s: %s at %d to %d\n", "cmscope", "setPolylinesBuffers", input, maxNumberOfPoints);

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);

    for (i = 0; i < block->insz[input]; i++)
    {
        pPolylineUID = getPolyline(pAxeUID, block, input, i);
        result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
    }

    return result;
}

static BOOL setPolylinesBounds(scicos_block * block, int input, int periodCounter)
{
    char const* pFigureUID;
    char *pAxeUID;

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

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);

    return setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
