/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_gui.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "createGraphicObject.h"
#include "BuildObjects.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "FigureList.h"
#include "HandleManagement.h"
#include "SetHashTable.h"
#include "stricmp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "sciprint.h"

#define COLOR_COMPONENT 3
/*--------------------------------------------------------------------------*/
void setDefaultProperties(int _iFig, BOOL _bAlreadyExist);
int addColor(int _iFig, double* _pdblColor);
/*--------------------------------------------------------------------------*/
int sci_figure(char * fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iFig = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iId = 0;
    int iPos = 0;
    int i = 0;


    //figure(num) -> scf(num)
    //figure() -> scf()

    //figure(x, "...", ...)
    // -> f = scf(x);
    //      set(f, "...", ...);

    if (iRhs % 2 == 0)
    {
        int i = 0;
        int iNewId = -1;
        int iAxes = 0;
        int* piAxes = &iAxes;

        //get higher value of winsid to create the new windows @ + 1
        int nbFigure = sciGetNbFigure();

        if (nbFigure)
        {
            int * ids = (int*)MALLOC(nbFigure * sizeof(int));

            if (ids == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            sciGetFiguresId(ids);

            //find higher value
            for (i = 0 ; i < nbFigure ; i++)
            {
                if (ids[i] > iNewId)
                {
                    iNewId = ids[i];
                }
            }
        }

        //use next value
        iNewId = iNewId + 1;

        //create a new window with id = iNewId
        iFig = createNewFigureWithAxes();
        setGraphicObjectProperty(iFig, __GO_ID__, &iNewId, jni_int, 1);
        setCurrentFigure(iFig);

        getGraphicObjectProperty(iFig, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxes);
        setCurrentSubWin(iAxes);

        //setting up new figure
        setDefaultProperties(iFig, FALSE);
    }
    else
    {
        //create or set current figure to x

        int iAxes = 0;
        int* piAxes = &iAxes;
        double dblId = 0;
        //figure(x);
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarMatrixType(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddr, &dblId))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iId = (int)(dblId + 0.5); //avoid 1.999 -> 1

        //get current fig from id
        iFig = getFigureFromIndex(iId);
        if (iFig == 0)
        {
            // No Figure available with this index, should create it  !!
            iFig = createNewFigureWithAxes();
            setGraphicObjectProperty(iFig, __GO_ID__, &iId, jni_int, 1);
        }

        setCurrentFigure(iFig);
        getGraphicObjectProperty(iFig, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxes);
        setCurrentSubWin(iAxes);

        //setting up new figure
        setDefaultProperties(iFig, FALSE);

        iPos = 1;
    }

    if (iRhs > 1)
    {
        //set(iFig, iPos, iPos + 1)
        for (i = iPos + 1 ; i <= iRhs ; i += 2)
        {
            int isMatrixOfString = 0;
            int* piAddrProp = NULL;
            char* pstProName = NULL;
            int* piAddrData = NULL;
            int iRows = 0;
            int iCols = 0;
            void* _pvData = NULL;
            int iType = 0;

            //get property name
            sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrProp);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddrProp, &pstProName))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
                return 1;
            }

            //check property value to compatibility
            if (stricmp(pstProName, "backgroundcolor") == 0)
            {
                freeAllocatedSingleString(pstProName);
                pstProName = strdup("background");
            }
            else if (stricmp(pstProName, "foregroundcolor") == 0)
            {
                freeAllocatedSingleString(pstProName);
                pstProName = strdup("foreground");
            }

            //get address of value on stack
            sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddrData);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
                return 1;
            }

            getVarType(pvApiCtx, piAddrData, &iType);

            if ((strcmp(pstProName, "user_data") == 0) || (stricmp(pstProName, "userdata") == 0))
            {
                /* in this case set_user_data_property
                * directly uses the  third position in the stack
                * to get the variable which is to be set in
                * the user_data property (any data type is allowed) S. Steer */
                _pvData = (void*)piAddrData;         /*position in the stack */
                iRows = -1;   /*unused */
                iCols = -1;   /*unused */
                iType = -1;
            }
            else
            {
                switch (iType)
                {
                    case sci_matrix :
                        getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, (double**)&_pvData);
                        break;
                    case sci_boolean :
                        getMatrixOfBoolean(pvApiCtx, piAddrData, &iRows, &iCols, (int**)&_pvData);
                        break;
                    case sci_handles :
                        getMatrixOfHandle(pvApiCtx, piAddrData, &iRows, &iCols, (long long**)&_pvData);
                        break;
                    case sci_strings :
                        if (   strcmp(pstProName, "tics_labels") != 0 && strcmp(pstProName, "auto_ticks") != 0 &&
                                strcmp(pstProName, "axes_visible") != 0 && strcmp(pstProName, "axes_reverse") != 0 &&
                                strcmp(pstProName, "text") != 0 && stricmp(pstProName, "string") != 0 &&
                                stricmp(pstProName, "tooltipstring") != 0) /* Added for uicontrols */
                        {
                            if (getAllocatedSingleString(pvApiCtx, piAddrData, (char**)&_pvData))
                            {
                                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 3);
                                return 1;
                            }
                            iRows = (int)strlen((char*)_pvData);
                            iCols = 1;
                        }
                        else
                        {
                            isMatrixOfString = 1;
                            getAllocatedMatrixOfString(pvApiCtx, piAddrData, &iRows, &iCols, (char***)&_pvData);
                        }
                        break;
                    case sci_list :
                        iCols = 1;
                        getListItemNumber(pvApiCtx, piAddrData, &iRows);
                        _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                        break;
                    default :
                        _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                        break;
                }

                callSetProperty(pvApiCtx, iFig, _pvData, iType, iRows, iCols, pstProName);
                if (iType == sci_strings)
                {
                    //free allacted data
                    if (isMatrixOfString == 1)
                    {
                        freeAllocatedMatrixOfString(iRows, iCols, (char**)_pvData);
                    }
                    else
                    {
                        freeAllocatedSingleString((char*)_pvData);
                    }
                }
            }
        }
    }

    //return new created fig
    createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
void setDefaultProperties(int _iFig, BOOL _bAlreadyExist)
{
    //get figure axes
    int iAxes = getOrCreateDefaultSubwin();
    int iDrawing = 0;
    int iColorIndex = 0;
    int iFilled = 0;
    int iAxesVisible = 0;

    setGraphicObjectProperty(_iFig, __GO_IMMEDIATE_DRAWING__, &iDrawing, jni_bool, 1);

    if (_bAlreadyExist == FALSE)
    {
        double pdblNewColor[COLOR_COMPONENT] = {0.8, 0.8, 0.8};
        iColorIndex = addColor(_iFig, pdblNewColor);

        //set background in figure and axes to new ( or existting ) color
        setGraphicObjectProperty(_iFig, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);
        setGraphicObjectProperty(iAxes, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);
    }

    //a.filled = "off"
    setGraphicObjectProperty(iAxes, __GO_FILLED__, &iFilled, jni_bool, 1);

    //a.axes_visible = "off"
    setGraphicObjectProperty(iAxes, __GO_X_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
    setGraphicObjectProperty(iAxes, __GO_Y_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
    setGraphicObjectProperty(iAxes, __GO_Z_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);

    //f.immediate_drawing = "on"
    iDrawing = 1;
    setGraphicObjectProperty(_iFig, __GO_IMMEDIATE_DRAWING__, &iDrawing, jni_bool, 1);
}
/*--------------------------------------------------------------------------*/
int addColor(int _iFig, double* _pdblNewColor)
{
    int iColorIndex = 0;
    int i = 0, j = 0;
    int iColorMapSize = 0;
    int* piColorMapSize = &iColorMapSize;
    double* pdblColorMap = NULL;
    //for new figure, we have to set figure and axes background to [0.8 0.8 0.8]
    //to do that, we have to update figure.colormap to add new color if not exist.
    //or get index of color in current color_map

    //first get figure.color_map
    getGraphicObjectProperty(_iFig, __GO_COLORMAP_SIZE__, jni_int, (void**)&piColorMapSize);
    getGraphicObjectProperty(_iFig, __GO_COLORMAP__, jni_double_vector, (void **)&pdblColorMap);

    //check if newColor already in coloMap
    for (i = 0 ; i < iColorMapSize ; i++)
    {
        BOOL bFound = TRUE;
        double* pdblCurrentColor = pdblColorMap + i;
        for (j = 0 ; j < COLOR_COMPONENT ; j++)
        {
            if (*(pdblCurrentColor + j * iColorMapSize) != _pdblNewColor[j])
            {
                bFound = FALSE;
                break;
            }
        }

        if (bFound)
        {
            iColorIndex = i + 1;
            break;
        }
    }

    //not found in current color map
    if (iColorIndex == 0)
    {
        int iNewColorMapSize = (iColorMapSize + 1) * COLOR_COMPONENT;
        double* pdblNewColorMap = (double*)MALLOC(sizeof(double) * iNewColorMapSize);
        //we have to add the new color at the end of the current color map
        for (i = 0 ; i < COLOR_COMPONENT ; i++)
        {
            memcpy(pdblNewColorMap + i * (iColorMapSize + 1),
                   pdblColorMap + (i * iColorMapSize),
                   iColorMapSize * sizeof(double));

            pdblNewColorMap[i * (iColorMapSize + 1) + iColorMapSize] = _pdblNewColor[i];
        }

        setGraphicObjectProperty(_iFig, __GO_COLORMAP__, pdblNewColorMap, jni_double_vector, iNewColorMapSize);

        iColorIndex = iColorMapSize + 1;
    }
    return iColorIndex;
}
/*--------------------------------------------------------------------------*/
