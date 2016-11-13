/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

/*------------------------------------------------------------------------
 *    Graphic library
 --------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "math_graphics.h"
#include "PloEch.h"
#include "Plot2d.h"

#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"
#include "isanan.h"

#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "Axes.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "Scierror.h"

#include "sci_malloc.h"             /* MALLOC */
#include "scitokenize.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"

#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "Sciwarning.h"

/*--------------------------------------------------------------------
 *  plot2dn(ptype,Logflags,x,y,n1,n2,style,strflag,legend,brect,aaint,lstr1,lstr2)
 *
 *  Draw *n1 curves of *n2 points each
 *
 *  ptype is an int which gives the polyline drawind mode (0,1,2,3,4)
 *
 *  Logflags is a two character string
 *
 *  (x[i+(*n2)*j] ,y[i+(*n2)*j]) Double values giving the point
 *  position of point i of curve j (i=0,*n2-1 j=0,*n1-1)
 *
 *  style[*n1]-> give the style to use for each curve
 *     if style is positive --> a mark is used (mark id = style[i])
 *     if style is strictly negative --> a dashed line is used
 *        (dash id = abs(style[i])
 *     if there's only one curve, style can be of type style[0]=style,
 *     style[1]=pos (pos in [1,6])
 *     pos give the legend position (1 to 6) (this can be iteresting
 *     if you want to superpose curves with different legends by
 *     calling plot2d more than one time.
 *
 *  strflag[3] is a string
 *
 *     if strflag[0] == '1' then legends are added
 *        legend = "leg1@leg2@....@legn"; gives the legend for each curve
 *      else no legend
 *
 *     if strflag[1] == '1' then  the values of brect are used to fix
 *        the drawing boundaries :  brect[]= <xmin,ymin,xmax,ymax>;
 *      if strflag[1] == '2' then the values  are computed from data
 *      else if strflag[1]=='0' the previous values
 *                (previous call or defaut values) are used
 *
 *     if  strflag[2] == '1' ->then an axis is added
 *        the number of intervals
 *        is specified by the vector aaint[4] of integers
 *         <aaint[0],aaint[1]> specifies the x-axis number of  points
 *         <aaint[2],aaint[3]> same for y-axis
 *     if  strflag[2] == '2' -> no axis, only a box around the curves
 *     else no box and no axis

 * lstr* : unused (but used by Fortran)
 *--------------------------------------------------------------------------*/

int plot2dn(int ptype, char *logflags, double *x, double *y, int *n1, int *n2, int *style, char *strflag, char *legend, double *brect, int *aaint,
            BOOL flagNax, int lstr1, int lstr2)
{
    int iSubwinUID = 0;
    int iCurFigureUID = 0;
    int closeflag = 0;
    int jj = 0;
    long hdl = 0;
    int *pObj = NULL;
    int cmpt = 0;
    int with_leg = 0;
    double drect[6];
    char dataflag = 0;

    BOOL bounds_changed = FALSE;
    BOOL axes_properties_changed = FALSE;

    double rotationAngles[2];
    int clipState = 0;
    int autoScale = 0;
    int logFlags[3];
    int iTmp = 0;
    int *piTmp = &iTmp;
    char textLogFlags[3];
    int firstPlot = 0;
    int newFirstPlot = 0;
    int autoSubticks = 0;

    iSubwinUID = getOrCreateDefaultSubwin();

    /*
     * Check if the auto_clear property is on and then erase everything
     * To be implemented
     */
    checkRedrawing();

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iSubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force logflags to those given by argument */

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piTmp);
    firstPlot = iTmp;

    /* Reset x and y logflags */
    if (firstPlot)
    {
        logFlags[0] = getBooleanLogFlag(logflags[1]);
        logFlags[1] = getBooleanLogFlag(logflags[2]);

        setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOG_FLAG__, &logFlags[0], jni_bool, 1);
        setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOG_FLAG__, &logFlags[1], jni_bool, 1);
    }

    /* Forces "clipgrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    autoScale = iTmp;

    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])
        {
            case '0':
                /* do not change data bounds */
                break;
            case '1':
            case '3':
            case '5':
            case '7':
                /* Force data bounds=brect */
                re_index_brect(brect, drect);
                break;
            case '2':
            case '4':
            case '6':
            case '8':
            case '9':
                /* Force data bounds to the x and y bounds */
                if ((int)strlen(logflags) < 1)
                {
                    dataflag = 'g';
                }
                else
                {
                    dataflag = logflags[0];
                }

                getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piTmp);
                logFlags[0] = iTmp;
                getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piTmp);
                logFlags[1] = iTmp;
                getGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piTmp);
                logFlags[2] = iTmp;

                /* Conversion required by compute_data_bounds2 */
                textLogFlags[0] = getTextLogFlag(logFlags[0]);
                textLogFlags[1] = getTextLogFlag(logFlags[1]);
                textLogFlags[2] = getTextLogFlag(logFlags[2]);

                compute_data_bounds2(0, dataflag, textLogFlags, x, y, *n1, *n2, drect);

                break;
        }

        /* merge data bounds and drect */
        if (!firstPlot && (strflag[1] == '5' || strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9'))
        {
            double *dataBounds;

            getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

            drect[0] = Min(dataBounds[0], drect[0]);    /*xmin */
            drect[2] = Min(dataBounds[2], drect[2]);    /*ymin */
            drect[1] = Max(dataBounds[1], drect[1]);    /*xmax */
            drect[3] = Max(dataBounds[3], drect[3]);    /*ymax */
        }

        if (strflag[1] != '0')
        {
            bounds_changed = update_specification_bounds(iSubwinUID, drect, 2);
        }
    }

    if (firstPlot)
    {
        bounds_changed = TRUE;
    }

    /* Adapted to the MVC */
    axes_properties_changed = strflag2axes_properties(iSubwinUID, strflag);

    /* just after strflag2axes_properties */
    newFirstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &newFirstPlot, jni_bool, 1);

    with_leg = (strflag[0] == '1');

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
     * u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    autoSubticks = !flagNax;
    setGraphicObjectProperty(iSubwinUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

    if (flagNax == TRUE)
    {
        getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piTmp);
        logFlags[0] = iTmp;
        getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piTmp);
        logFlags[1] = iTmp;

        if (logFlags[0] == 0 && logFlags[1] == 0)
        {
            int autoTicks = 0;
            int i = 0;
            int iSize = 0;
            double dblFabs = 0;
            char** stringVector = NULL;

            if (aaint[1] == -1)
            {
                autoTicks = 1;
                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            }
            else if (aaint[1] == 0)
            {
                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_TICKS_LOCATIONS__, NULL, jni_double_vector, 0);
                autoTicks = 0;
                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            }
            else
            {
                double* dXGrads = (double*) MALLOC(aaint[1] * sizeof(double));

                // Compute X grads
                dXGrads[0] = drect[0];
                if (aaint[1] > 1)
                {
                    double pas = (drect[1] - drect[0]) / (aaint[1] - 1);
                    for (i = 0; i < aaint[1]; i++)
                    {
                        dXGrads[i] = drect[0] + pas * i;
                    }
                }

                autoTicks = 0;
                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_TICKS_LOCATIONS__, dXGrads, jni_double_vector, aaint[1]);
                // Create X Labels
                stringVector = (char **) MALLOC(aaint[1] * sizeof(char*));
                for (i = 0; i < aaint[1]; i++)
                {
                    iSize = 6;
                    if (dXGrads[i] < 0)
                    {
                        iSize += 2;
                    }
                    dblFabs = fabs(dXGrads[i]);
                    if (dblFabs >= 10)
                    {
                        iSize = iSize + (int)floor(log10(dblFabs));
                    }

                    stringVector[i] = (char*) MALLOC(iSize * sizeof(char));
                    sprintf(stringVector[i], "%.3f", dXGrads[i]);
                    stringVector[i][iSize - 1] = '\0';
                }

                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, aaint[1]);

                for (i = 0; i < aaint[1]; i++)
                {
                    FREE(stringVector[i]);
                }

                FREE(stringVector);
                FREE(dXGrads);
                stringVector = NULL;
            }

            if (aaint[3] == -1)
            {
                autoTicks = 1;
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            }
            else if (aaint[3] == 0)
            {
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_TICKS_LOCATIONS__, NULL, jni_double_vector, 0);
                autoTicks = 0;
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            }
            else
            {
                double* dYGrads = (double*) MALLOC(aaint[3] * sizeof(double));

                // Compute Y grads
                dYGrads[0] = drect[2];
                if (aaint[3] > 1)
                {
                    double pas = (drect[3] - drect[2]) / (aaint[3] - 1);
                    for (i = 0; i < aaint[3]; i++)
                    {
                        dYGrads[i] = drect[2] + pas * i;
                    }
                }

                autoTicks = 0;
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_TICKS_LOCATIONS__, dYGrads, jni_double_vector, aaint[3]);

                // Create Y Labels
                stringVector = (char**) MALLOC(aaint[3] * sizeof(char*));
                for (i = 0; i < aaint[3]; i++)
                {
                    iSize = 6;
                    if (dYGrads[i] < 0)
                    {
                        iSize += 2;
                    }
                    dblFabs = fabs(dYGrads[i]);
                    if (dblFabs >= 10)
                    {
                        iSize = iSize + (int)floor(log10(dblFabs));
                    }
                    stringVector[i] = (char*) MALLOC(iSize * sizeof(char));
                    sprintf(stringVector[i], "%.3f", dYGrads[i]);
                    stringVector[i][iSize - 1] = '\0';
                }

                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, aaint[3]);

                for (i = 0; i < aaint[3]; i++)
                {
                    FREE(stringVector[i]);
                }

                FREE(stringVector);
                FREE(dYGrads);
                stringVector = NULL;
            }

            // X and Y subticks
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_SUBTICKS__, aaint, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_SUBTICKS__, &aaint[2], jni_int, 1);
        }
        else
        {
            Sciwarning(_("Warning: Nax does not work with logarithmic scaling.\n"));
        }

    }

    /*---- Drawing the curves and the legends ----*/
    if (*n1 != 0)
    {
        if ((pObj = (int*)MALLOC((*n1 + 1) * sizeof(int))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "plot2d");
            return -1;
        }

        /*A.Djalel 3D axes */
        for (jj = 0; jj < *n1; jj++)
        {
            int iObjUID = 0;

            if (style[jj] > 0)
            {
                BOOL isline = TRUE;

                if (ptype == 3)
                {
                    isline = FALSE;
                }
                iObjUID = ConstructPolyline(getCurrentSubWin(), &(x[jj * (*n2)]),
                                            &(y[jj * (*n2)]), PD0, closeflag, *n2, ptype,
                                            &style[jj], NULL, NULL, NULL, NULL, isline, FALSE, FALSE, FALSE);
            }
            else
            {
                int minusstyle = -style[jj];

                iObjUID = ConstructPolyline(getCurrentSubWin(), &(x[jj * (*n2)]),
                                            &(y[jj * (*n2)]), PD0, closeflag, *n2, ptype,
                                            NULL, NULL, &minusstyle, NULL, NULL, FALSE, FALSE, TRUE, FALSE);
            }
            if (iObjUID == 0)
            {
                // skip
                Scierror(999, _("%s: No more memory.\n"), "plot2d");
            }
            else
            {
                setCurrentObject(iObjUID);

                pObj[cmpt] = iObjUID;
                cmpt++;
            }

        }

        /*---- Drawing the Legends ----*/
        if (with_leg)
        {
            int iLegUID = 0;
            char **Str;
            int nleg;

            if (scitokenize(legend, &Str, &nleg))
            {
                FREE(pObj);
                Scierror(999, _("%s: No more memory.\n"), "plot2d");
                return 0;
            }

            iLegUID = ConstructLegend(getCurrentSubWin(), Str, pObj, Min(nleg, cmpt));

            if (iLegUID != 0)
            {
                int legendLocation;
                int contourMode;

                /* 9: LOWER_CAPTION */
                legendLocation = 9;
                setGraphicObjectProperty(iLegUID, __GO_LEGEND_LOCATION__, &legendLocation, jni_int, 1);

                contourMode = 0;

                setGraphicObjectProperty(iLegUID, __GO_FILL_MODE__, &contourMode, jni_bool, 1);
                setGraphicObjectProperty(iLegUID, __GO_LINE_MODE__, &contourMode, jni_bool, 1);
            }

            freeArrayOfString(Str, nleg);
        }

        /*---- construct Compound ----*/
        if (cmpt > 0)
        {
            int parentVisible = 0;
            int *piParentVisible = &parentVisible;
            int iCompoundUID = createCompound(iSubwinUID, pObj, cmpt);
            setCurrentObject(iCompoundUID);
        }
        FREE(pObj);

    }
    /* End of the curves and legend block */

    return 0;
}

/* Given two set of coordinates x and y this routine computes the corresponding
 *  data bounds rectangle drect=[xmin,ymin,xmax,ymax] taking into account the logflag
 *  -> means we have to find among the data the min > 0.
 */
void compute_data_bounds2(int cflag, char dataflag, char *logflags, double *x, double *y, int n1, int n2, double *drect)
{
    int size_x = 0, size_y = 0;
    double xd[2];
    double *x1 = NULL;

    switch (dataflag)
    {
        case 'e':
            xd[0] = 1.0;
            xd[1] = (double)n2;
            x1 = xd;
            size_x = (n2 != 0) ? 2 : 0;
            break;
        case 'o':
            x1 = x;
            size_x = n2;
            break;
        case 'g':
        default:
            x1 = x;
            size_x = (cflag == 1) ? n1 : (n1 * n2);
            break;
    }

    if (size_x != 0)
    {
        if (logflags[0] != 'l')
        {
            MiniMaxi(x1, size_x, drect, drect + 1);
            //drect[0] = Mini(x1, size_x);
            //drect[1] = Maxi(x1, size_x);
        }
        else
        {
            /* log. case */
            drect[0] = sciFindStPosMin(x1, size_x);
            drect[1] = Maxi(x1, size_x);
        }

    }
    else
    {
        if (logflags[0] != 'l')
        {
            drect[0] = 0.0;
            drect[1] = 10.0;
        }
        else
        {
            /* log. case */
            drect[0] = 1.0;
            drect[1] = 10.0;
        }
    }

    size_y = (cflag == 1) ? n2 : (n1 * n2);
    if (size_y != 0)
    {
        if (logflags[1] != 'l')
        {
            MiniMaxi(y, size_y, drect + 2, drect + 3);
            //drect[2] = Mini(y, size_y);
            //drect[3] = Maxi(y, size_y);
        }
        else
        {
            /* log. case */
            drect[2] = sciFindStPosMin(y, size_y);
            drect[3] = Maxi(y, size_y);
        }
    }
    else
    {
        if (logflags[1] != 'l')
        {
            drect[2] = 0.0;
            drect[3] = 10.0;
        }
        else
        {
            /* log. case */
            drect[2] = 1.0;
            drect[3] = 10.0;
        }
    }

    /* back to default values for  x=[] and y = [] */
    if (drect[2] == LARGEST_REAL || drect[3] == -LARGEST_REAL || C2F(isanan)(&drect[2]) || C2F(isanan)(&drect[3]))
    {
        if (logflags[1] != 'l')
        {
            drect[2] = 0.0;
        }
        else
        {
            drect[2] = 1.0;
        }

        drect[3] = 10.0;
    }

    if (drect[0] == LARGEST_REAL || drect[1] == -LARGEST_REAL || C2F(isanan)(&drect[0]) || C2F(isanan)(&drect[1]))
    {
        if (logflags[0] != 'l')
        {
            drect[0] = 0.0;
        }
        else
        {
            drect[0] = 1.0;
        }

        drect[1] = 10.0;
    }
}

BOOL update_specification_bounds(int iSubwinUID, double rect[6], int flag)
{
    double *dataBounds = NULL;

    /*
     * 2D: keep the existing zmin and zmax
     * which is why they must be fetched
     */
    if (flag != 3)
    {
        getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);
        
        if (dataBounds)
        {
            rect[4] = dataBounds[4];
            rect[5] = dataBounds[5];
            releaseGraphicObjectProperty(__GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
        }
        else
        {
            return FALSE;
        }
    }

    setGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, rect, jni_double_vector, 6);

    return TRUE;
}

/* F.Leray */
/* brect must have the same format as drect i.e.: [xmin,xmax,ymin,ymax] */
/* brect = INPUT ; drect = OUTPUT (warning drect is dim 6) */
int re_index_brect(double *brect, double *drect)
{
    drect[0] = brect[0];
    drect[1] = brect[2];
    drect[2] = brect[1];
    drect[3] = brect[3];

    return 0;
}

/* F.Leray 07.05.04 */
/* Dispatch info contained in strflag to all the flag available in
   sciSubwin object (tight_limits, isoview, isaxes...) */
/*
 * This function has been adapted to the MVC framework
 */
BOOL strflag2axes_properties(int iSubwinUID, char *strflag)
{
    BOOL haschanged = FALSE;
    BOOL xTightLimitsPrev = FALSE, yTightLimitsPrev = FALSE, zTightLimitsPrev = FALSE;
    BOOL isoviewPrev = FALSE;
    int boxPrev = 0;
    int tightLimits = 0, xTightLimits = 0, yTightLimits = 0, zTightLimits = 0;
    int firstPlot = 0;
    int axisVisible = 0;
    int boxType = 0;
    int xLocationPrev = 0;
    int yLocationPrev = 0;
    int xLocation = 0;
    int yLocation = 0;
    int isoview = 0;
    int axesVisiblePrev[3];
    int axesVisible[3];

    int iTmp = 0;
    int *piTmp = &iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisiblePrev[0] = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisiblePrev[1] = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisiblePrev[2] = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, jni_int, (void**)&piTmp);
    boxPrev = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOCATION__, jni_int, (void**)&piTmp);
    xLocationPrev = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, jni_int, (void**)&piTmp);
    yLocationPrev = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_X_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    xTightLimitsPrev = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    yTightLimitsPrev = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Z_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    zTightLimitsPrev = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_ISOVIEW__, jni_bool, (void **)&piTmp);
    isoviewPrev = iTmp;

    /* F.Leray 07.05.04 */
    /* strflag[1] Isoview & tight_limits flags management */
    switch (strflag[1])
    {
        case '0':
        case '9':
            /* no changes */
            break;
        case '1':
        case '2':
        case '7':
        case '8':
            tightLimits = 1;
            setGraphicObjectProperty(iSubwinUID, __GO_X_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Z_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            break;
        case '3':
        case '4':
            isoview = 1;
            setGraphicObjectProperty(iSubwinUID, __GO_ISOVIEW__, &isoview, jni_bool, 1);
            break;
        case '5':
        case '6':
            /* pretty axes */
            tightLimits = 0;
            setGraphicObjectProperty(iSubwinUID, __GO_X_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Z_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);
            break;
    }

    /* F.Leray 07.05.04 */
    /* strflag[2] */
    switch (strflag[2])
    {

        case '0':
            getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piTmp);
            firstPlot = iTmp;

            if (firstPlot)
            {
                axisVisible = 0;
                /* 0: OFF */
                boxType = 0;

                setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
                setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
                /* also trigger z axis */
                setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
                setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);
            }
            /*else no changes : the isaxes properties is driven by the previous plot */
            break;
        case '1':
            axisVisible = 1;
            /* 1: ON */
            boxType = 1;
            /* 4: LEFT */
            yLocation = 4;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, &yLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

            break;
        case '2':
            axisVisible = 0;
            boxType = 1;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            /* also trigger z axis */
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

            break;
        case '3':
            axisVisible = 1;
            boxType = 0;
            /* 5: RIGHT */
            yLocation = 5;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, &yLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

            break;
        case '4':
            axisVisible = 1;
            /* 2: MIDDLE */
            xLocation = 2;
            yLocation = 2;
            boxType = 0;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            /* also trigger z axis */
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOCATION__, &xLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, &yLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

            break;
        case '5':
            axisVisible = 1;
            xLocation = 2;
            yLocation = 2;
            boxType = 1;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            /* also trigger z axis */
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOCATION__, &xLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, &yLocation, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

            break;
        case '9':
            axisVisible = 1;

            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
            /* also trigger z axis */
            setGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
    }

    getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisible[0] = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisible[1] = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Z_AXIS_VISIBLE__, jni_bool, (void **)&piTmp);
    axesVisible[2] = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_BOX_TYPE__, jni_int, (void**)&piTmp);
    boxType = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOCATION__, jni_int, (void**)&piTmp);
    xLocation = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOCATION__, jni_int, (void**)&piTmp);
    yLocation = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_X_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    xTightLimits = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Y_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    yTightLimits = iTmp;
    getGraphicObjectProperty(iSubwinUID, __GO_Z_TIGHT_LIMITS__, jni_bool, (void **)&piTmp);
    zTightLimits = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_ISOVIEW__, jni_bool, (void **)&piTmp);
    isoview = iTmp;

    /* Find if something has changed */
    if (axesVisible[0] != axesVisiblePrev[0]
            || axesVisible[1] != axesVisiblePrev[1]
            || axesVisible[2] != axesVisiblePrev[2]
            || xLocation != xLocationPrev || yLocation != yLocationPrev || boxType != boxPrev
            || xTightLimits != xTightLimitsPrev || yTightLimits != yTightLimitsPrev || zTightLimits != zTightLimitsPrev
            || isoview != isoviewPrev)
    {
        haschanged = TRUE;
    }
    else
    {
        haschanged = FALSE;
    }

    return haschanged;
}
