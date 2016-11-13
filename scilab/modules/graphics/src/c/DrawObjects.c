/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
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
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to Draw the content of a window.
 *    The main functions is sciDrawObj that draws the objects recursively.
 ------------------------------------------------------------------------/-*/

#include <stdio.h>
#include <string.h>

#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "PloEch.h"
#include "sciprint.h"
#include "math_graphics.h"
#include "Format.h"
#include "HandleManagement.h"

#include "sci_malloc.h" /* MALLOC */
#include "localization.h"

#include "math.h" /* fabs, floor, log10, pow */
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Sciwarning.h"
#include "Scierror.h"

//#include "../../../tclsci/includes/GedManagement.h"

/* get the displayed bounds of an axis */
void sciGetDisplayedBounds(int iSubWinUID,
                           double      * xmin   ,
                           double      * xmax   ,
                           double      * ymin   ,
                           double      * ymax   ,
                           double      * zmin   ,
                           double      * zmax)
{
    /*****************************************************************
     * get initial bounds
    *****************************************************************/

    int iZoomEnabled = 0;
    int* piZoomEnabled = &iZoomEnabled;
    double* bounds = NULL;
    int iLogFlag = 0;
    int *piLogFlag = &iLogFlag;

    getGraphicObjectProperty(iSubWinUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&piZoomEnabled);
    if (iZoomEnabled)
    {
        getGraphicObjectProperty(iSubWinUID, __GO_ZOOM_BOX__, jni_double_vector, (void **)&bounds);
    }
    else
    {
        getGraphicObjectProperty(iSubWinUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&bounds);
    }

    if (!bounds)
    {
        Scierror(999, _("Could not retrive bounds.\n"));
        return;
    }

    *xmin = bounds[0];
    *xmax = bounds[1];
    *ymin = bounds[2];
    *ymax = bounds[3];
    *zmin = bounds[4];
    *zmax = bounds[5];

    /*****************************************************************
     * modify  bounds and aaint  if using log scaling X axis
     *****************************************************************/

    getGraphicObjectProperty(iSubWinUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (iLogFlag == 1)
    {
        if (sciGetLogExponent(*xmin, *xmax, xmin, xmax) != 0)
        {
            Sciwarning(_("Warning: Can't use Log on X-axis xmin is negative.\n"));
        }
    }

    /*****************************************************************
     * modify  bounds and aaint  if using log scaling Y axis
     *****************************************************************/

    getGraphicObjectProperty(iSubWinUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (iLogFlag == 1)
    {
        if (sciGetLogExponent(*ymin, *ymax, ymin, ymax) != 0)
        {
            Sciwarning(_("Warning: Can't use Log on Y-axis ymin is negative.\n"));
        }
    }

    /*****************************************************************
     * modify  bounds and aaint  if using log scaling Z axis
     *****************************************************************/

    getGraphicObjectProperty(iSubWinUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (iLogFlag == 1)
    {
        if (sciGetLogExponent(*zmin, *zmax, zmin, zmax) != 0)
        {
            Sciwarning(_("Warning: Can't use Log on Z-axis zmin is negative.\n"));
        }
    }
}
