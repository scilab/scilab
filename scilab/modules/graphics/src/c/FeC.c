/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2005-2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2001 -  Bruno Pincon (for gain in speed and added
 *    possibilities to set zmin, zmax by the user and also to set the
 *    first and last color of the colormap)
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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

/**
 * for entities handling
 */

#include "machine.h"
#include "math_graphics.h"
#include "Axes.h"

#include "SetProperty.h"
#include "BuildObjects.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PloEch.h"
#include "Plot2d.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"

#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "Sciwarning.h"

/**
 * Before Scilab 5.1, default colout was [-1, -1].
 * However, to be more coherent with other fec object properties
 * (which were interfaced from Scilab in version 5.1),
 * the default value is now [0,0].
 * However, to keep backward compatibility, we made a patch for
 * the fec function.
 * In other word, this function converts colout from fec function
 * colout argument to fec handle outside_colors properties.
 */
static void coloutPatch(int colout[2]);

/*------------------------------------------------------------
 *  Iso contour with grey level or colors
 *  for a function defined by finite elements
 *  (f is linear on triangles)
 *  we give two versions of the function :
 *     - a quick version wich only fill triangles according to the average
 *     value of f on a triangle (no more such version now ?)
 *     - and a slow version but more sexy which use the fact that f is linear
 *     on each triangle.
 *  Nodes (x[no],y[no])
 *  Triangles (Matrix: [ numero, no1,no2,no3,iflag;...]
 *  func[no] : Function value on Nodes.
 *  Nnode : number of nodes
 *  Ntr   : number of triangles
 *  strflag,legend,brect,aint : see plot2d
 *  zminmax   : to set (optionaly) the min and max level
 *  colminmax : to set (optionaly) the first and last color to use
 *
 *  modified by Bruno Pincon 01/02/2001 for gain in speed and added
 *  possibilities to set zmin, zmax by the user and also to set the
 *  first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr)
---------------------------------------------------------------*/

int C2F(fec)(double *x, double *y, double *triangles, double *func, int *Nnode, int *Ntr, int *Nvertex,
             char *strflag, char *legend, double *brect, int *aaint, double *zminmax,
             int *colminmax, int *colout, BOOL with_mesh, BOOL flagNax, int lstr1, int lstr2)
{
    int n1 = 1;

    /* Fec code */
    int iSubwinUID = 0;
    int iFecUID = 0;
    int iParentCompoundUID = 0;

    int cmpt = 0;
    double drect[6];

    BOOL bounds_changed = FALSE;
    BOOL axes_properties_changed = FALSE;

    char textLogFlags[3];
    int clipState = 0;
    int autoScale = 0;
    int *piAutoScale = &autoScale;
    int firstPlot = 0;
    int *piFirstPlot = &firstPlot;
    int logFlags[3];
    int autoSubticks = 0;
    int iTmp = 0;
    int *piTmp = &iTmp;
    double rotationAngles[2];

    iSubwinUID = getCurrentSubWin();

    checkRedrawing();

    rotationAngles[0] = 0.0;
    rotationAngles[1] = 270.0;

    setGraphicObjectProperty(iSubwinUID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

    /* Force "cligrf" clipping (1) */
    clipState = 1;
    setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, jni_bool, (void **)&piFirstPlot);

    getGraphicObjectProperty(iSubwinUID, __GO_AUTO_SCALE__, jni_bool, (void **)&piAutoScale);

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

                compute_data_bounds2(0, 'g', textLogFlags, x, y, n1, *Nnode, drect);
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

    /* just after strflag2axes_properties */
    firstPlot = 0;
    setGraphicObjectProperty(iSubwinUID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

    /* F.Leray 07.10.04 : trigger algo to init. manual graduation u_xgrads and
    u_ygrads if nax (in matdes.c which is == aaint HERE) was specified */

    /* The MVC AUTO_SUBTICKS property corresponds to !flagNax */
    /* store new value for flagNax */
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

    /* Construct the object */
    /* Patch on colout */
    /* For coherence with other properties, default colout is [0, 0] for fec handles instead of  */
    /* [-1,-1] */
    coloutPatch(colout);
    iFecUID = ConstructFec(iSubwinUID, x, y, triangles, func,
                           *Nnode, *Ntr, *Nvertex, zminmax, colminmax, colout, with_mesh);

    if (iFecUID == 0)
    {
        // error in allocation
        Scierror(999, _("%s: No more memory.\n"), "fec");
        return -1;
    }

    /* Set fec as current */
    setCurrentObject(iFecUID);

    iParentCompoundUID = createCompound(iSubwinUID, &iFecUID, 1);
    setCurrentObject(iParentCompoundUID);  /** construct Compound **/
    return 0;

}
/*--------------------------------------------------------------------------*/
static void coloutPatch(int colout[2])
{
    if (colout[0] < 0)
    {
        /* default mode */
        colout[0] = 0;
    }
    else if (colout[0] == 0)
    {
        /* transparent facet */
        colout[0] = -1;
    }

    if (colout[1] < 0)
    {
        /* default mode */
        colout[1] = 0;
    }
    else if (colout[1] == 0)
    {
        /* transparent facet */
        colout[1] = -1;
    }

}
/*--------------------------------------------------------------------------*/
