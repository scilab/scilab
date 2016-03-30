/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2005 - INRIA - Fabrice Leray
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
#include <string.h>
#include "math_graphics.h"
#include "PloEch.h"

#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "Axes.h"
#include "Scierror.h"
#include "sciprint.h"
#include "GrayPlot.h"

#include "sci_malloc.h" /* MALLOC */
#include "localization.h"
#include "Plot2d.h"
#include "get_ticks_utils.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "Sciwarning.h"

int C2F(xgray)(double *x, double *y, double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, char *logflags, long int l1)
{
    int iSubwinUID = 0;
    int iGrayplotUID = 0;
    double xx[2], yy[2];
    int nn1 = 1, nn2 = 2;
    double drect[6];
    BOOL bounds_changed = FALSE;
    BOOL isRedrawn = FALSE;
    BOOL axes_properties_changed = FALSE;

    char textLogFlags[3];
    double rotationAngles[2];
    int clipState = 0;
    int autoScale = 0;
    int firstPlot = 0;
    int logFlags[3];
    char dataflag = 0;
    int autoSubticks = 0;

    int iTmp = 0;
    int* piTmp = &iTmp;

    xx[0] = Mini(x, *n1);
    xx[1] = Maxi(x, *n1);
    yy[0] = Mini(y, *n2);
    yy[1] = Maxi(y, *n2);

    /* Adding F.Leray 22.04.04 */
    iSubwinUID = getCurrentSubWin();

    isRedrawn = checkRedrawing();

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iSubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piTmp);
    firstPlot = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    autoScale = iTmp;

    /* Reset x and y logflags */
    if (firstPlot)
    {
        logFlags[0] = getBooleanLogFlag(logflags[1]);
        logFlags[1] = getBooleanLogFlag(logflags[2]);

        setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_LOG_FLAG__, &logFlags[0], jni_bool, 1);
        setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_LOG_FLAG__, &logFlags[1], jni_bool, 1);
    }

    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])
        {
            case '0':
                /* do not change data bounds */
                break;
            case '1' :
            case '3' :
            case '5' :
            case '7':
                /* Force data bounds=brect */
                re_index_brect(brect, drect);
                break;
            case '2' :
            case '4' :
            case '6' :
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

                /* Force data bounds to the x and y bounds */
                compute_data_bounds2(0, dataflag, textLogFlags, xx, yy, nn1, nn2, drect);
                break;
        }

        /* merge data bounds and drect */
        if (!firstPlot && (strflag[1] == '7' || strflag[1] == '8'))
        {
            double* dataBounds;
            getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

            drect[0] = Min(dataBounds[0], drect[0]); /*xmin*/
            drect[2] = Min(dataBounds[2], drect[2]); /*ymin*/
            drect[1] = Max(dataBounds[1], drect[1]); /*xmax*/
            drect[3] = Max(dataBounds[3], drect[3]); /*ymax*/
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

    axes_properties_changed = strflag2axes_properties(iSubwinUID, strflag);

    firstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
    u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    autoSubticks = !flagNax;
    setGraphicObjectProperty(iSubwinUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

    if (flagNax == TRUE)
    {
        if (logFlags[0] == 0 && logFlags[1] == 0)
        {
            int autoTicks;

            autoTicks = 0;
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
        }
        else
        {
            Sciwarning(_("Warning: Nax does not work with logarithmic scaling.\n"));
        }
    }

    /* Constructs the object */
    iGrayplotUID = ConstructGrayplot(getCurrentSubWin(), x, y, z, *n1, *n2, 0);

    /* Failed allocation */
    if (iGrayplotUID == 0)
    {
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    /* Sets the grayplot as current */
    setCurrentObject(iGrayplotUID);

    return (0);
}

int C2F(implot)(unsigned char *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1, int plottype)
{
    int iSubwinUID = 0;
    int iGrayplotUID = 0;
    double xx[2], yy[2];
    static int nn1 = 1, nn2 = 2;
    double drect[6];
    BOOL bounds_changed = FALSE;
    BOOL axes_properties_changed = FALSE;

    char textLogFlags[3];
    double rotationAngles[2];
    int clipState = 0;
    int autoScale = 0;
    int firstPlot = 0;
    int logFlags[3];
    int autoSubticks = 0;

    int iTmp = 0;
    int* piTmp = &iTmp;

    xx[0] = 0.5;
    xx[1] = *n2 + 0.5;
    yy[0] = 0.5;
    yy[1] = *n1 + 0.5;

    /* Adding F.Leray 22.04.04 */
    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iSubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piTmp);
    firstPlot = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    autoScale = iTmp;

    /*---- Boundaries of the frame ----*/
    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])
        {
            case '0':
                /* do not change data bounds */
                break;
            case '1' :
            case '3' :
            case '5' :
            case '7':
                /* Force data bounds=brect */
                re_index_brect(brect, drect);
                break;
            case '2' :
            case '4' :
            case '6' :
            case '8':
            case '9':
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

                compute_data_bounds2(0, 'g', textLogFlags, xx, yy, nn1, nn2, drect);
                break;
        }

        /* merge data bounds and drect */
        if (!firstPlot &&
                (strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9'))
        {
            double* dataBounds;
            getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

            drect[0] = Min(dataBounds[0], drect[0]); /*xmin*/
            drect[2] = Min(dataBounds[2], drect[2]); /*ymin*/
            drect[1] = Max(dataBounds[1], drect[1]); /*xmax*/
            drect[3] = Max(dataBounds[3], drect[3]); /*ymax*/
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

    axes_properties_changed = strflag2axes_properties(iSubwinUID, strflag);

    firstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
    u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    autoSubticks = !flagNax;
    setGraphicObjectProperty(iSubwinUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

    if (flagNax == TRUE)
    {
        if (logFlags[0] == 0 && logFlags[1] == 0)
        {
            int autoTicks;

            autoTicks = 0;
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
        }
        else
        {
            Sciwarning(_("Warning: Nax does not work with logarithmic scaling.\n"));
        }
    }

    /* Construct the grayplot object */
    iGrayplotUID = ConstructImplot(iSubwinUID, NULL, z, *n1 + 1, *n2 + 1, plottype);

    if (iGrayplotUID == 0)
    {
        // allocation error
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    setCurrentObject(iGrayplotUID);
    /* if the auto_clear is on we must redraw everything */

    return 0;
}

int C2F(xgray1)(double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1)
{
    int iSubwinUID = 0;
    int iGrayplotUID = 0;
    double xx[2], yy[2];
    static int nn1 = 1, nn2 = 2;
    double drect[6];
    BOOL bounds_changed = FALSE;
    BOOL axes_properties_changed = FALSE;

    char textLogFlags[3];
    double rotationAngles[2];
    int clipState = 0;
    int autoScale = 0;
    int firstPlot = 0;
    int logFlags[3];
    int autoSubticks = 0;

    int iTmp = 0;
    int* piTmp = &iTmp;

    xx[0] = 0.5;
    xx[1] = *n2 + 0.5;
    yy[0] = 0.5;
    yy[1] = *n1 + 0.5;

    /* Adding F.Leray 22.04.04 */
    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iSubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piTmp);
    firstPlot = iTmp;

    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piTmp);
    autoScale = iTmp;

    /*---- Boundaries of the frame ----*/
    if (autoScale)
    {
        /* compute and merge new specified bounds with the data bounds */
        switch (strflag[1])
        {
            case '0':
                /* do not change data bounds */
                break;
            case '1' :
            case '3' :
            case '5' :
            case '7':
                /* Force data bounds=brect */
                re_index_brect(brect, drect);
                break;
            case '2' :
            case '4' :
            case '6' :
            case '8':
            case '9':
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

                compute_data_bounds2(0, 'g', textLogFlags, xx, yy, nn1, nn2, drect);
                break;
        }

        /* merge data bounds and drect */
        if (!firstPlot &&
                (strflag[1] == '7' || strflag[1] == '8' || strflag[1] == '9'))
        {
            double* dataBounds;
            getGraphicObjectProperty(iSubwinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

            drect[0] = Min(dataBounds[0], drect[0]); /*xmin*/
            drect[2] = Min(dataBounds[2], drect[2]); /*ymin*/
            drect[1] = Max(dataBounds[1], drect[1]); /*xmax*/
            drect[3] = Max(dataBounds[3], drect[3]); /*ymax*/
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

    axes_properties_changed = strflag2axes_properties(iSubwinUID, strflag);

    firstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
    u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    autoSubticks = !flagNax;
    setGraphicObjectProperty(iSubwinUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);



    if (flagNax == TRUE)
    {
        if (logFlags[0] == 0 && logFlags[1] == 0)
        {
            int autoTicks;

            autoTicks = 0;
            setGraphicObjectProperty(iSubwinUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
        }
        else
        {
            Sciwarning(_("Warning: Nax does not work with logarithmic scaling.\n"));
        }
    }

    /* Construct the grayplot object */
    iGrayplotUID = ConstructGrayplot(iSubwinUID, NULL, NULL, z, *n1 + 1, *n2 + 1, 1);

    if (iGrayplotUID == 0)
    {
        // allocation error
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    setCurrentObject(iGrayplotUID);
    /* if the auto_clear is on we must redraw everything */

    return 0;
}


/*-------------------------------------------------------
 * like xgray1 :
 * but xrect here give the rectangle in which the
 * grayplot is to be drawn using the current scale
 -------------------------------------------------------*/

int C2F(xgray2)(double *z, int *n1, int *n2, double *xrect)
{
    int iSubwinUID = 0;
    int iGrayplotUID = 0;
    BOOL isRedrawn = FALSE;
    double y; /* void for ConstructGrayplot */
    int clipState = 0;
    int firstPlot = 0;

    isRedrawn = checkRedrawing();

    /*---- Boundaries of the frame ----*/
    iSubwinUID = getCurrentSubWin();

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    iGrayplotUID = ConstructGrayplot(iSubwinUID, xrect, &y, z, *n1 + 1, *n2 + 1, 2);

    if (iGrayplotUID == 0)
    {
        // allocation error
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    setCurrentObject(iGrayplotUID);
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    return (0);
}

/*
  For matplot1
*/
int C2F(implot1)(unsigned char *z, int *n1, int *n2, double *xrect, int plottype)
{
    int iSubwinUID = 0;
    int iGrayplotUID = 0;
    BOOL isRedrawn = FALSE;
    double y = 0; /* void for ConstructGrayplot */
    int clipState = 0;
    int firstPlot = 0;

    isRedrawn = checkRedrawing();

    /*---- Boundaries of the frame ----*/
    iSubwinUID = getCurrentSubWin();

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    iGrayplotUID = ConstructImplot(iSubwinUID, xrect, z, *n1 + 1, *n2 + 1, plottype);
    if (iGrayplotUID == 0)
    {
        // allocation error
        Scierror(999, _("%s: No more memory.\n"), "grayplot");
        return -1;
    }

    setCurrentObject(iGrayplotUID);
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    return (0);
}
