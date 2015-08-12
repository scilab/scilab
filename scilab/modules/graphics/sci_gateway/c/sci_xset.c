/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "XsetXgetParameters.h"
#include "Format.h"
#include "msgs.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "createGraphicObject.h"
#include "CurrentObject.h"
#include "BuildObjects.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "AxesModel.h"
#include "getGraphicObjectProperty.h"
#include "deleteGraphicObject.h"
#include "warningmode.h"
#include "sciprint.h"

/*--------------------------------------------------------------------------*/
int sci_xset(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    char* l1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;
    int* piAddrlr = NULL;
    double* lr = NULL;

    int m1 = 0, m2 = 0, xm[5], xn[5], x[5] = {0, 0, 0, 0, 0}, i = 0, v = 0;
    double  xx[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int iSubwinUID = 0;
    BOOL keyFound = FALSE;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 6);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a string at position 1.
    if (getAllocatedSingleString(pvApiCtx, piAddrl1, &l1))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    for (i = 0 ; i < NUMSETFONC ; i++)
    {
        if (strcmp(l1, KeyTab_[i]) == 0)
        {
            keyFound = TRUE;
            break;
        }
    }

    if (!keyFound)
    {
        freeAllocatedSingleString(l1);
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, (l1));
        return 0;
    }

    // in case of "fpf" and "auto clear", the second argument must be a string
    if (strcmp(l1, "fpf") == 0 || strcmp(l1, "auto clear") == 0)
    {
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }

        /* second argument is not a scalar it must be a string */
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(l1);
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a string at position 2.
        if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
        {
            freeAllocatedSingleString(l1);
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
            return 1;
        }

        if (strcmp(l1, "fpf") == 0)
        {
            strcpy(getFPF(), l2);
        }
        else// if (strcmp(l1, "auto clear") == 0)
        {
            int bAutoClear = (int) FALSE;

            if (strcmp(l2, "on") == 0)
            {
                bAutoClear = (int) TRUE;
            }

            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, &bAutoClear, jni_bool, 1);
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        freeAllocatedSingleString(l1);
        freeAllocatedSingleString(l2);
        return 0;
    }

    // in default case, only one input argument expected.
    if (strcmp(l1, "default") == 0)
    {
        if (nbInputArgument(pvApiCtx) != 1)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 1);
            return 1;
        }
    }

    // in other case than "fpf" and "auto clear", the second to fifth argument must be a matrix of double
    for (i = 2 ; i <= nbInputArgument(pvApiCtx) ; i++)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrlr);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(l1);
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position i.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrlr, &xm[i - 2], &xn[i - 2], &lr);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(l1);
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, i);
            return 1;
        }

        x[i - 2] = (int)lr[0];
        xx[i - 2] = lr[0];
    }

    if (strcmp((l1), "clipping") == 0)
    {
        int clipState = 2;
        if (nbInputArgument(pvApiCtx) != 5 && nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 2, 5);
            return 1;
        }

        if (nbInputArgument(pvApiCtx) == 2)
        {
            int i = 0;
            int iRows = 0;
            int iCols = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrlr, &iRows, &iCols, &lr);
            if (sciErr.iErr)
            {
                freeAllocatedSingleString(l1);
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
                return 1;
            }


            if (iRows * iCols != 4)
            {
                freeAllocatedSingleString(l1);
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-element vector expected.\n"), fname, 2, 4);
                return 1;
            }

            for (i = 0; i < 4 ; i++)
            {
                xx[i] = lr[i];
            }
        }
        iSubwinUID = getOrCreateDefaultSubwin();
        setGraphicObjectProperty(iSubwinUID, __GO_CLIP_BOX__, xx, jni_double_vector, 4);
        setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if (strcmp((l1), "colormap") == 0)
    {
        int iFigureUID = 0;
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }

        getOrCreateDefaultSubwin();
        iFigureUID = getCurrentFigure();
        setGraphicObjectProperty(iFigureUID , __GO_COLORMAP__, (lr), jni_double_vector, *xm * (*xn));
    }
    else if (strcmp((l1), "mark size") == 0)
    {
        int markSize = (int) xx[0];
        int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
        iSubwinUID = getOrCreateDefaultSubwin();

        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }

        setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);
        setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    }
    else if (strcmp((l1), "mark") == 0)
    {
        int markStyle = (int) xx[0];
        int markSize = (int) xx[1];
        int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
        int markMode = 1;
        if (nbInputArgument(pvApiCtx) != 3)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 3);
            return -1;
        }

        iSubwinUID = getOrCreateDefaultSubwin();
        setGraphicObjectProperty(iSubwinUID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
        setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1); /* force switch to tabulated mode : old syntax */
        setGraphicObjectProperty(iSubwinUID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
        setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    }
    else if (strcmp((l1), "font size") == 0)
    {
        double fontSize = xx[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
    }
    else if (strcmp((l1), "default") == 0)
    {
        // default color map
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

        int piFigurePosition[2] = {200, 200};
        int piFigureSize[2]     = {500, 500};
        int piAxesSize[2]       = {498, 366};
        int piViewPort[2]       = {0, 0};
        int piEmptyMatrix[4]    = {1, 0, 0, 0};

        // Create new axes and set it in current figure
        char* pSubWinUID = NULL;

        // init variables
        int iZero   = 0;
        BOOL bTrue  = TRUE;
        BOOL bFalse = FALSE;
        int m       = NUMCOLORS_SCI;
        int i       = 0;
        int iCopy   = 3;

        int defaultBackground = -2;

        double* pdblColorMap = NULL;
        int iFigureUID = 0;

        getFPF()[0] = '\0';

        pdblColorMap = (double*)malloc(m * 3 * sizeof(double));
        if (pdblColorMap == NULL)
        {
            freeAllocatedSingleString(l1);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        // Create figure if it not exist.
        iFigureUID = getCurrentFigure();
        if (iFigureUID == 0)
        {
            iFigureUID = createNewFigureWithAxes();
            setCurrentFigure(iFigureUID);
            AssignOutputVariable(pvApiCtx, 1) = 0;
            ReturnArguments(pvApiCtx);
            free(pdblColorMap);
            freeAllocatedSingleString(l1);
            return 0;
        }

        iSubwinUID = getCurrentSubWin();
        if (iSubwinUID != 0)
        {
            int iChildrenCount  = 0;
            int* childrencount  = &iChildrenCount;
            int* piChildrenUID  = NULL;
            int iHidden         = 0;
            int *piHidden       = &iHidden;

            getGraphicObjectProperty(iFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);
            getGraphicObjectProperty(iFigureUID, __GO_CHILDREN__, jni_int_vector, (void **)&piChildrenUID);

            for (i = 0; i < childrencount[0]; ++i)
            {
                getGraphicObjectProperty(piChildrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                if (iHidden == 0)
                {
                    deleteGraphicObject(piChildrenUID[i]);
                }
            }
        }

        cloneAxesModel(iFigureUID);

        // Set default figure properties
        setGraphicObjectProperty(iFigureUID, __GO_POSITION__, piFigurePosition, jni_int_vector, 2);
        setGraphicObjectProperty(iFigureUID, __GO_SIZE__, piFigureSize, jni_int_vector, 2);
        setGraphicObjectProperty(iFigureUID, __GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);
        setGraphicObjectProperty(iFigureUID, __GO_AUTORESIZE__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(iFigureUID, __GO_VIEWPORT__, piViewPort, jni_int_vector, 2);
        setGraphicObjectProperty(iFigureUID, __GO_NAME__, _("Figure n°%d"), jni_string, 1);
        setGraphicObjectProperty(iFigureUID, __GO_INFO_MESSAGE__, "", jni_string, 1);
        setGraphicObjectProperty(iFigureUID, __GO_PIXEL_DRAWING_MODE__, &iCopy, jni_int, 1);
        setGraphicObjectProperty(iFigureUID, __GO_ANTIALIASING__, &iZero, jni_int, 1);
        setGraphicObjectProperty(iFigureUID, __GO_IMMEDIATE_DRAWING__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(iFigureUID, __GO_BACKGROUND__, &defaultBackground, jni_int, 1);
        setGraphicObjectProperty(iFigureUID, __GO_VISIBLE__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(iFigureUID, __GO_ROTATION_TYPE__, &iZero, jni_int, 1);
        setGraphicObjectProperty(iFigureUID, __GO_EVENTHANDLER__, "", jni_string, 1);
        setGraphicObjectProperty(iFigureUID, __GO_EVENTHANDLER_ENABLE__, &bFalse, jni_bool, 1);
        setGraphicObjectProperty(iFigureUID, __GO_USER_DATA__, piEmptyMatrix, jni_int_vector, 4);
        setGraphicObjectProperty(iFigureUID, __GO_RESIZEFCN__, "", jni_string, 1);
        setGraphicObjectProperty(iFigureUID, __GO_TAG__, "", jni_string, 1);

        for (i = 0; i < m; i++)
        {
            pdblColorMap[i]         = (double)(defcolors[3 * i] / 255.0);
            pdblColorMap[i + m]     = (double)(defcolors[3 * i + 1] / 255.0);
            pdblColorMap[i + 2 * m] = (double)(defcolors[3 * i + 2] / 255.0);
        }

        setGraphicObjectProperty(iFigureUID, __GO_COLORMAP__, pdblColorMap, jni_double_vector, 3 * m);
        setParentObject(iFigureUID, 0);

    }
    else if (strcmp((l1), "clipgrf") == 0)
    {
        int clipState = 1;
        /* special treatement for xset("cligrf") */
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if (strcmp((l1), "clipoff") == 0)
    {
        int clipState = 0;
        /* special treatement for xset("clipoff") */
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if (strcmp((l1), "hidden3d") == 0)
    {
        /* special treatement for xset("hidden3d") */
        int hiddenColor = (int) x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);
    }
    else if (strcmp((l1), "font") == 0)
    {
        int fontStyle = (int) xx[0];
        double fontSize = xx[1];
        if (nbInputArgument(pvApiCtx) != 3)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 3);
            return -1;
        }

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_STYLE__, &fontStyle, jni_int, 1);
    }
    else if (strcmp((l1), "window") == 0 || strcmp((l1), "figure") == 0)
    {
        // Find if window already exists, if not create a new one
        int iID = x[0];
        int iFigureUID = getFigureFromIndex(iID);
        int iAxesUID = 0;
        int* piAxesUID = &iAxesUID;

        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }

        if (iFigureUID == 0)
        {
            iFigureUID = createNewFigureWithAxes();
            setGraphicObjectProperty(iFigureUID, __GO_ID__, &iID, jni_int, 1);
        }

        setCurrentFigure(iFigureUID);
        getGraphicObjectProperty(iFigureUID, __GO_SELECTED_CHILD__, jni_int,  (void**)(&piAxesUID));
        setCurrentSubWin(iAxesUID);

    }
    else if ((strcmp((l1), "foreground") == 0) || (strcmp((l1), "color") == 0) || (strcmp((l1), "pattern") == 0))
    {
        int iColor = (int) x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_COLOR__, &iColor, jni_int, 1);
    }
    else if (strcmp((l1), "background") == 0)
    {
        int iColor = (int) x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_BACKGROUND__, &iColor, jni_int, 1);
    }
    else if (strcmp((l1), "thickness") == 0)
    {
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        sciSetLineWidth(getOrCreateDefaultSubwin(), x[0]);
    }
    else if (strcmp((l1), "line style") == 0)
    {
        int lineStyle = (int) x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
    }
    else if (strcmp((l1), "dashes") == 0)
    {
        int lineStyle = (int) x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
    }
    else if (strcmp((l1), "wresize") == 0)
    {
        int iAutoResizeMode = x[0];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTORESIZE__, &iAutoResizeMode, jni_bool, 1);
    }
    else if (strcmp((l1), "wpos") == 0)
    {
        int figurePosition[2];
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return -1;
        }
        getOrCreateDefaultSubwin();

        figurePosition[0] = x[0];
        figurePosition[1] = x[1];
        setGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, figurePosition, jni_int_vector, 2);
    }
    else if (strcmp((l1), "wpdim") == 0 || strcmp((l1), "wdim") == 0)
    {
        int figureSize[2];
        if (nbInputArgument(pvApiCtx) != 2 && nbInputArgument(pvApiCtx) != 3)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 2, 3);
            return -1;
        }

        /* Xwindows limits dimensions to 2^16 */
        if ((x[0] > 65535) || (x[1] > 65535))
        {
            x[0] = Min(x[0], 65535);
            x[1] = Min(x[1], 65535);
            i = 106;
            v = 0;
            C2F(msgs)(&i, &v);
        }

        getOrCreateDefaultSubwin();

        figureSize[0] = x[0];
        figureSize[1] = x[1];
        setGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, figureSize, jni_int_vector, 2);
    } /*Ajout A.Djalel le 10/11/03 */
    else if (strcmp((l1), "viewport") == 0)
    {
        int viewport[4] = {x[0], x[1], 0, 0};
        if (nbInputArgument(pvApiCtx) != 3)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 3);
            return 1;
        }
        getOrCreateDefaultSubwin();
        setGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, viewport, jni_int_vector, 2);
    }
    else if (strcmp((l1), "line mode") == 0)
    {
        int iZero = 0;
        int iOne = 1;
        iSubwinUID = getOrCreateDefaultSubwin();
        if (nbInputArgument(pvApiCtx) != 2)
        {
            freeAllocatedSingleString(l1);
            Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
            return 1;
        }

        if (x[0] == 0)
        {
            setGraphicObjectProperty(iSubwinUID, __GO_LINE_MODE__, &iZero, jni_bool, 1);
        }
        else
        {
            setGraphicObjectProperty(iSubwinUID, __GO_LINE_MODE__, &iOne, jni_bool, 1);
        }
    }
    else
    {
        freeAllocatedSingleString(l1);
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, (l1));
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    freeAllocatedSingleString(l1);
    return 0;
}
/*--------------------------------------------------------------------------*/
