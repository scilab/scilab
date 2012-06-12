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

#include "gw_graphics.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "XsetXgetParameters.h"
#include "Format.h"
#include "ObjectSelection.h"
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
/*--------------------------------------------------------------------------*/
int xsetg(char * str, char * str1, int lx0, int lx1) ;
/*--------------------------------------------------------------------------*/
int sci_xset( char *fname, unsigned long fname_len )
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, xm[5], xn[5], x[5], i = 0, v = 0;
    int lr = 0;
    double  xx[5];
    sciPointObj *subwin = NULL;
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

        if(Rhs == 2)
        {
            int i = 0;
            int lr = 0;
            int iRows = 0;
            int iCols = 0;

            GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &lr);

            if(iRows * iCols != 4)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-element vector expected.\n"), fname, 2, 4);
                return 1;
            }

            for(i = 0; i < 4 ; i++)
            {
                xx[i] = *stk(lr + i);
            }
        }
        subwinUID = getOrCreateDefaultSubwin();
        setGraphicObjectProperty(subwinUID, __GO_CLIP_BOX__, xx, jni_double_vector, 4);
        setGraphicObjectProperty(subwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);
    }
    else if ( strcmp(cstk(l1), "colormap") == 0)
    {
        char *pFigureUID = NULL;
        getOrCreateDefaultSubwin();
        pFigureUID = getCurrentFigure();
        setGraphicObjectProperty(pFigureUID, __GO_COLORMAP__, stk(lr), jni_double_vector, *xm * (*xn));
    }
    else if ( strcmp(cstk(l1), "mark size") == 0)
    {
        int markSize = (int) xx[0];
        int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
        char *subwinUID = getOrCreateDefaultSubwin();

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

        subwinUID = getOrCreateDefaultSubwin();
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
        // FIXME
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
        char *pFigureUID = getFigureFromIndex(iID);
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
        sciSetLineWidth(getOrCreateDefaultSubwin(), x[0]);
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
        /* a supprimer ce n'est pas une propriete mais une action */
        showPixmap((char*)sciGetParentFigure(subwin));
    }
    else if (strcmp(cstk(l1), "viewport") == 0)
    {
        int viewport[4] = {x[0], x[1], 0, 0};
        getOrCreateDefaultSubwin();
        setGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, viewport, jni_int_vector, 2);
    }
    else if (strcmp(cstk(l1), "wwpc") == 0)
    {
        // clear pixmap
        clearPixmap(sciGetParentFigure(subwin));
    }
    else if (strcmp(cstk(l1), "line mode") == 0)
    {
        char *pstSubwinUID = getOrCreateDefaultSubwin();
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
