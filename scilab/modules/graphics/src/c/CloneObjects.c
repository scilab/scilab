/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
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
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to CLONE an object, it means make
 *    a copy of an object under the same subwin.
 --------------------------------------------------------------------------*/

#include "CloneObjects.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "SetProperty.h"
#include "HandleManagement.h"

#include "sci_malloc.h" /* MALLOC */
#include "localization.h"
#include "Scierror.h"
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

int clonePolyline(int iSource)
{
    int iClonedPolylineUID = 0;
    int iParentAxes = 0;
    int* piParentAxes = &iParentAxes;

    double* dataX = NULL;
    double* dataY = NULL;
    double* dataZ = NULL;

    double lineThickness = 0.0;
    double* pLineThickness = &lineThickness;

    int nPoints = 0;
    int* pNPoints = &nPoints;

    int closed = 0;
    int* pClosed = &closed;

    int polylineStyle = 0;
    int* pPolylineStyle = &polylineStyle;

    int tmp = 0;
    int* pTmp = &tmp;

    int lineStyle = 0;
    int foreground = 0;
    int background = 0;
    int markForeground = 0;
    int markBackground = 0;
    int markStyle = 0;

    int lineMode = 0;
    int fillMode = 0;
    int markMode = 0;
    int interpShaded = 0;

    getGraphicObjectProperty(iSource, __GO_PARENT_AXES__, jni_int, (void **)&piParentAxes);

    getGraphicObjectProperty(iSource, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
    getGraphicObjectProperty(iSource, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
    getGraphicObjectProperty(iSource, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

    getGraphicObjectProperty(iSource, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void **)&pNPoints);

    getGraphicObjectProperty(iSource, __GO_CLOSED__, jni_bool, (void **)&pClosed);
    getGraphicObjectProperty(iSource, __GO_POLYLINE_STYLE__, jni_int, (void **)&pPolylineStyle);

    /* ContouredObject properties */
    getGraphicObjectProperty(iSource, __GO_LINE_THICKNESS__, jni_double, (void **)&pLineThickness);
    getGraphicObjectProperty(iSource, __GO_LINE_STYLE__, jni_int, (void **)&pTmp);
    lineStyle = tmp;
    getGraphicObjectProperty(iSource, __GO_LINE_COLOR__, jni_int, (void **)&pTmp);
    foreground = tmp;
    getGraphicObjectProperty(iSource, __GO_BACKGROUND__, jni_int, (void **)&pTmp);
    background = tmp;
    getGraphicObjectProperty(iSource, __GO_MARK_STYLE__, jni_int, (void **)&pTmp);
    markStyle = tmp;
    getGraphicObjectProperty(iSource, __GO_MARK_FOREGROUND__, jni_int, (void **)&pTmp);
    markForeground = tmp;
    getGraphicObjectProperty(iSource, __GO_MARK_BACKGROUND__, jni_int, (void **)&pTmp);
    markBackground = tmp;

    getGraphicObjectProperty(iSource, __GO_LINE_MODE__, jni_bool, (void **)&pTmp);
    lineMode = tmp;
    getGraphicObjectProperty(iSource, __GO_FILL_MODE__, jni_bool, (void **)&pTmp);
    fillMode = tmp;
    getGraphicObjectProperty(iSource, __GO_MARK_MODE__, jni_bool, (void **)&pTmp);
    markMode = tmp;
    getGraphicObjectProperty(iSource, __GO_INTERP_COLOR_MODE__, jni_bool, (void **)&pTmp);
    interpShaded = tmp;

    iClonedPolylineUID = ConstructPolyline(iParentAxes, dataX, dataY, dataZ, closed, nPoints, polylineStyle,
                                           &foreground, &background, &markStyle, &markForeground, &markBackground,
                                           lineMode, fillMode, markMode, interpShaded);

    /* These properties must be additionally set as this is not done by allocatePolyline */
    setGraphicObjectProperty(iClonedPolylineUID, __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
    setGraphicObjectProperty(iClonedPolylineUID, __GO_LINE_THICKNESS__, &lineThickness, jni_double, 1);

    /*
     * Some these properties are passed by value thus do not care to release them
     * and do not call releaseGraphicObjectProperty on purpose.
     *
     * releaseGraphicObjectProperty has a real impact for the following :
     *  - jni_string
     *  - jni_double_vector out of the DATA_MODEL
     */

    return iClonedPolylineUID;
}

