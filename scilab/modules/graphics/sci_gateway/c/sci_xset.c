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
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include "gw_graphics.h"
#include "stack-c.h"
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
int xsetg(char * str, char * str1, int lx0, int lx1) ;
/*--------------------------------------------------------------------------*/
int sci_xset( char *fname, unsigned long fname_len )
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, xm[5], xn[5], x[5] = {0, 0, 0, 0, 0}, i = 0, v = 0;
    int lr = 0;
    double  xx[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    char * subwinUID = NULL;
    BOOL keyFound = FALSE ;

    if (Rhs <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckRhs(1, 6);
    CheckLhs(0, 1);

    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);

    for ( i = 0 ; i < NUMSETFONC ; i++ )
    {
        if ( strcmp(cstk(l1), KeyTab_[i]) == 0 )
        {
            keyFound = TRUE ;
            break ;
        }
    }

    if ( !keyFound )
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
        return 0;
    }

    /* Allan CORNET Avril 2004 */
    /* Bloque la commande xset('window') sans numero de fenetre */
    if (Rhs == 1 && (strcmp(cstk(l1), "window") == 0) )
    {
        Scierror(999, _("%s : '%s' must be set\n"), fname, "window-number");
        return 0;
    }

    if (Rhs == 2 && VarType(2) != sci_matrix)
    {
        /* second argument is not a scalar it must be a string */
        GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2);
        xsetg(cstk(l1), cstk(l2), m1, m2);
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    if (Rhs == 1 && strcmp(cstk(l1), "default") == 0)
    {
        /* first treatment for xsetg : then we continue */
        xsetg(cstk(l1), "void", m1, 4L);
    }

    for ( i = 2 ; i <= Rhs ; i++ )
    {
        GetRhsVar(i, MATRIX_OF_DOUBLE_DATATYPE, &xm[i - 2], &xn[i - 2], &lr);
        x[i - 2] = (int)  * stk(lr);
        xx[i - 2] = *stk(lr);
    }

    if (strcmp(cstk(l1), "wdim") == 0 || strcmp(cstk(l1), "wpdim") == 0)
    {
        /* Xwindows limits dimensions to 2^16 */
        if ( (x[0] > 65535) || (x[1] > 65535))
        {
            x[0] = Min(x[0], 65535);
            x[1] = Min(x[1], 65535);
            i = 106;
            v = 0;
            C2F(msgs)(&i, &v);
        }
    }

    if (strcmp(cstk(l1), "clipping") == 0)
    {
        int clipState = 2;
        if (Rhs != 5 && Rhs != 2)
        {
            Scierror(999, _("%s: Wrong number of input argument: %d or %d expected.\n"), fname, 2, 5);
            return 1;
        }

        if (Rhs == 2)
        {
            int i = 0;
            int lr = 0;
            int iRows = 0;
            int iCols = 0;

            GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &lr);

            if (iRows * iCols != 4)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-element vector expected.\n"), fname, 2, 4);
                return 1;
            }

            for (i = 0; i < 4 ; i++)
            {
                xx[i] = *stk(lr + i);
            }
        }
        subwinUID = (char*)getOrCreateDefaultSubwin();
        setGraphicObjectProperty(subwinUID, __GO_CLIP_BOX__, xx, jni_double_vector, 4);
        setGraphicObjectProperty(subwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "colormap") == 0)
    {
        char *pFigureUID = NULL;
        getOrCreateDefaultSubwin();
        pFigureUID = (char*)getCurrentFigure();
        setGraphicObjectProperty(pFigureUID, __GO_COLORMAP__, stk(lr), jni_double_vector, *xm * (*xn));
    }
    else if ( strcmp(cstk(l1), "mark size") == 0)
    {
        int markSize = (int) xx[0];
        int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
        char *subwinUID = (char*)getOrCreateDefaultSubwin();

        setGraphicObjectProperty(subwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);
        setGraphicObjectProperty(subwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "mark") == 0)
    {
        int markStyle = (int) xx[0];
        int markSize = (int) xx[1];
        int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
        char *subwinUID = NULL;
        if (Rhs != 3)
        {
            Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), fname, 3);
            return -1;
        }

        subwinUID = (char*)getOrCreateDefaultSubwin();
        setGraphicObjectProperty(subwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1); /* force switch to tabulated mode : old syntax */
        setGraphicObjectProperty(subwinUID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
        setGraphicObjectProperty(subwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "font size") == 0)
    {
        double fontSize = xx[0];

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
    }
    else if ( strcmp(cstk(l1), "default") == 0 )
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
        char* pSubWinUID = (char*)getCurrentSubWin();

        // init variables
        int iZero   = 0;
        BOOL bTrue  = TRUE;
        BOOL bFalse = FALSE;
        int m       = NUMCOLORS_SCI;
        int i       = 0;
        int iCopy   = 3;

        int defaultBackground = -2;
        char error_message[70];

        double* pdblColorMap = (double*)malloc(m * 3 * sizeof(double));

        // Create figure if it not exist.
        char* pFigureUID = (char*)getCurrentFigure();
        if (pFigureUID == NULL)
        {
            pFigureUID = createNewFigureWithAxes();
            setCurrentFigure(pFigureUID);
            LhsVar(1) = 0;
            PutLhsVar();
            free(pdblColorMap);
            return 0;
        }

        if (pdblColorMap == NULL)
        {
            sprintf(error_message, _("%s: No more memory.\n"), "xset");
            return 0;
        }

        if (pSubWinUID != NULL)
        {
            int iChildrenCount  = 0;
            int* childrencount  = &iChildrenCount;
            char** childrenUID  = NULL;
            int iHidden         = 0;
            int *piHidden       = &iHidden;

            getGraphicObjectProperty(pFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);
            getGraphicObjectProperty(pFigureUID, __GO_CHILDREN__, jni_string_vector, (void **)&childrenUID);

            for (i = 0; i < childrencount[0]; ++i)
            {
                getGraphicObjectProperty(childrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                if (iHidden == 0)
                {
                    deleteGraphicObject(childrenUID[i]);
                }
            }
        }

        cloneAxesModel(pFigureUID);

        // Set default figure properties
        setGraphicObjectProperty(pFigureUID, __GO_POSITION__, piFigurePosition, jni_int_vector, 2);
        setGraphicObjectProperty(pFigureUID, __GO_SIZE__, piFigureSize, jni_int_vector, 2);
        setGraphicObjectProperty(pFigureUID, __GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);
        setGraphicObjectProperty(pFigureUID, __GO_AUTORESIZE__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(pFigureUID, __GO_VIEWPORT__, piViewPort, jni_int_vector, 2);
        setGraphicObjectProperty(pFigureUID, __GO_NAME__, _("Figure n°%d"), jni_string, 1);
        setGraphicObjectProperty(pFigureUID, __GO_INFO_MESSAGE__, "", jni_string, 1);
        setGraphicObjectProperty(pFigureUID, __GO_PIXMAP__, &bFalse, jni_bool, 1);
        setGraphicObjectProperty(pFigureUID, __GO_PIXEL_DRAWING_MODE__, &iCopy, jni_int, 1);
        setGraphicObjectProperty(pFigureUID, __GO_ANTIALIASING__, &iZero, jni_int, 1);
        setGraphicObjectProperty(pFigureUID, __GO_IMMEDIATE_DRAWING__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(pFigureUID, __GO_BACKGROUND__, &defaultBackground, jni_int, 1);
        setGraphicObjectProperty(pFigureUID, __GO_VISIBLE__, &bTrue, jni_bool, 1);
        setGraphicObjectProperty(pFigureUID, __GO_ROTATION_TYPE__, &iZero, jni_int, 1);
        setGraphicObjectProperty(pFigureUID, __GO_EVENTHANDLER__, "", jni_string, 1);
        setGraphicObjectProperty(pFigureUID, __GO_EVENTHANDLER_ENABLE__, &bFalse, jni_bool, 1);
        setGraphicObjectProperty(pFigureUID, __GO_USER_DATA__, piEmptyMatrix, jni_int_vector, 4);
        setGraphicObjectProperty(pFigureUID, __GO_RESIZEFCN__, "", jni_string, 1);
        setGraphicObjectProperty(pFigureUID, __GO_TAG__, "", jni_string, 1);

        for (i = 0; i < m; i++)
        {
            pdblColorMap[i]         = (double)(defcolors[3 * i] / 255.0);
            pdblColorMap[i + m]     = (double)(defcolors[3 * i + 1] / 255.0);
            pdblColorMap[i + 2 * m] = (double)(defcolors[3 * i + 2] / 255.0);
        }

        setGraphicObjectProperty(pFigureUID, __GO_COLORMAP__, pdblColorMap, jni_double_vector, 3 * m);
        setGraphicObjectProperty(pFigureUID, __GO_PARENT__, "", jni_string, 1);

    }
    else if ( strcmp(cstk(l1), "clipgrf") == 0 )
    {
        int clipState = 1;
        /* special treatement for xset("cligrf") */
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "clipoff") == 0 )
    {
        int clipState = 0;
        /* special treatement for xset("clipoff") */
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "hidden3d") == 0 )
    {
        /* special treatement for xset("hidden3d") */
        int hiddenColor = (int) x[0];

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "font") == 0)
    {
        int fontStyle = (int) xx[0];
        double fontSize = xx[1];
        if (Rhs != 3)
        {
            Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), fname, 3);
            return -1;
        }

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_STYLE__, &fontStyle, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "window") == 0 || strcmp(cstk(l1), "figure") == 0 )
    {
        // Find if window already exists, if not create a new one
        int iID = x[0];
        char *pFigureUID = (char*)getFigureFromIndex(iID);
        if (pFigureUID == NULL)
        {
            pFigureUID = createNewFigureWithAxes();
            setGraphicObjectProperty(pFigureUID, __GO_ID__, &iID, jni_int, 1);
            setCurrentFigure(pFigureUID);
        }
        setCurrentFigure(pFigureUID);
    }
    else if (( strcmp(cstk(l1), "foreground") == 0) || (strcmp(cstk(l1), "color") == 0) || ( strcmp(cstk(l1), "pattern") == 0) )
    {
        int iColor = (int) x[0];

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_COLOR__, &iColor, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "background") == 0)
    {
        int iColor = (int) x[0];

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_BACKGROUND__, &iColor, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "thickness") == 0)
    {
        sciSetLineWidth((char*)getOrCreateDefaultSubwin(), x[0]);
    }
    else if ( strcmp(cstk(l1), "line style") == 0)
    {
        int lineStyle = (int) x[0];
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "mark") == 0)
    {
        int markMode = 1;

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_MARK_MODE__, &markMode, jni_bool, 1);
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_MARK_STYLE__, x, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "colormap") == 0)
    {
        getOrCreateDefaultSubwin();
        setGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP__, stk(lr), jni_double_vector, xm[0] * xn[0]);
    }
    else if ( strcmp(cstk(l1), "dashes") == 0)
    {
        int lineStyle = (int) x[0];
        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "wresize") == 0)
    {
        int iAutoResizeMode = x[0];

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTORESIZE__, &iAutoResizeMode, jni_bool, 1);
    }
    else if ( strcmp(cstk(l1), "wpos") == 0)
    {
        int figurePosition[2];
        if (Rhs != 2)
        {
            Scierror(999, _("%s: Wrong number of input argument: %d expected.\n"), fname, 2);
            return -1;
        }
        getOrCreateDefaultSubwin();

        figurePosition[0] = x[0];
        figurePosition[1] = x[1];
        setGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, figurePosition, jni_int_vector, 2);
    }
    else if ( strcmp(cstk(l1), "wpdim") == 0 || strcmp(cstk(l1), "wdim") == 0)
    {
        int figureSize[2];
        if (Rhs != 2 && Rhs != 3)
        {
            Scierror(999, _("%s: Wrong number of input argument: %d or %d expected.\n"), fname, 2, 3);
            return -1;
        }
        getOrCreateDefaultSubwin();

        figureSize[0] = x[0];
        figureSize[1] = x[1];
        setGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, figureSize, jni_int_vector, 2);
    } /*Ajout A.Djalel le 10/11/03 */
    else if ( strcmp(cstk(l1), "pixmap") == 0)
    {
        int iPixmapMode = x[0];
        getOrCreateDefaultSubwin();

        setGraphicObjectProperty(getCurrentFigure(), __GO_PIXMAP__, &iPixmapMode, jni_bool, 1);
    }
    else if ( strcmp(cstk(l1), "wshow") == 0)
    {
        if (getWarningMode())
        {
            sciprint(_("WARNING: %s\n"), _("xset(\"wshow\") is obsolete."));
            sciprint(_("WARNING: %s\n"), _("It will be removed after Scilab 5.4.0."));
            sciprint(_("WARNING: %s\n"), _("Please use drawlater/drawnow instead."));
        }
    }
    else if (strcmp(cstk(l1), "viewport") == 0)
    {
        int viewport[4] = {x[0], x[1], 0, 0};
        getOrCreateDefaultSubwin();
        setGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, viewport, jni_int_vector, 2);
    }
    else if (strcmp(cstk(l1), "wwpc") == 0)
    {
        if (getWarningMode())
        {
            sciprint(_("WARNING: %s\n"), _("xset(\"wwpc\") is obsolete."));
            sciprint(_("WARNING: %s\n"), _("It will be removed after Scilab 5.4.0."));
            sciprint(_("WARNING: %s\n"), _("Please use drawlater/drawnow instead."));
        }
    }
    else if (strcmp(cstk(l1), "line mode") == 0)
    {
        char *pstSubwinUID = (char*)getOrCreateDefaultSubwin();
        int iZero = 0;
        int iOne = 1;
        if (x[0] == 0)
        {
            setGraphicObjectProperty(pstSubwinUID, __GO_LINE_MODE__, &iZero, jni_bool, 1);
        }
        else
        {
            setGraphicObjectProperty(pstSubwinUID, __GO_LINE_MODE__, &iOne, jni_bool, 1);
        }
    }
    else
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
int xsetg(char * str, char * str1, int lx0, int lx1)
{
    if ( strcmp(str, "fpf") == 0)
    {
        strcpy(getFPF(), str1);
    }
    else if ( strcmp(str, "auto clear") == 0)
    {
        int bAutoClear = (int) FALSE;

        if (strcmp(str1, "on") == 0)
        {
            bAutoClear = (int) TRUE;
        }

        setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, &bAutoClear, jni_bool, 1);
    }
    else if ( strcmp(str, "default") == 0)
    {
        getFPF()[0] = '\0';
    }
    else
    {
        Scierror(999, _("%s: Unrecognized input argument '%s'.\n"), "xset(arg,<string>)", str);
        return -1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
