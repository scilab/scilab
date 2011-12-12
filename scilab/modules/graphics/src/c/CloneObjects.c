/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "Scierror.h"
#include "BasicAlgos.h"

#include "createGraphicObject.h" /* cloneGraphicObject */
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
/*
 * Copies the ContouredObject properties from the source object to the destination object
 * This code ought to be moved into the Java Model, either in the relevant
 * constructor (ContouredObject) or into an initialization function.
 */
int cloneGraphicContext(char* sourceIdentifier, char* destIdentifier)
{
    double dblTmp = 0.0;
    double *pdblTmp = &dblTmp;
    int iTmp = 0;
    int *piTmp = &iTmp;

    int lineMode;
    int foreground;
    int lineStyle;
    int fillMode;
    int background;
    int markForeground;
    int markBackground;
    int markStyle;
    int markSize;
    int markSizeUnit;
    double lineThickness;

    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_MODE__, jni_bool, &piTmp);
    lineMode = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_COLOR__, jni_int, &piTmp);
    foreground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_THICKNESS__, jni_double, &pdblTmp);
    lineThickness = dblTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_LINE_STYLE__, jni_int, &piTmp);
    lineStyle = iTmp;

    /*
     * Commented out since there is a confusion between Axes' FILLED property
     * and the FILL_MODE ContouredObject property
     * To be corrected
     */
#if 0
    tmp = (int*) setGraphicObjectProperty(pobj->UID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);
    fillMode = *tmp;
#endif

    getGraphicObjectProperty(sourceIdentifier, __GO_BACKGROUND__, jni_int, &piTmp);
    background = iTmp;

    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_FOREGROUND__, jni_int, &piTmp);
    markForeground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_BACKGROUND__, jni_int, &piTmp);
    markBackground = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_STYLE__, jni_int, &piTmp);
    markStyle = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_SIZE__, jni_int, &piTmp);
    markSize = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_MARK_SIZE_UNIT__, jni_int, &piTmp);
    markSizeUnit = iTmp;

    setGraphicObjectProperty(destIdentifier, __GO_LINE_MODE__, &lineMode, jni_bool, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_COLOR__, &foreground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_THICKNESS__, &lineThickness, jni_double, 1);
    setGraphicObjectProperty(destIdentifier, __GO_LINE_STYLE__, &lineStyle, jni_int, 1);

    /* Commented out due to the confusion between Axes' FILLED and the FILL_MODE Contoured property */
#if 0
    setGraphicObjectProperty(destIdentifier, __GO_FILL_MODE__, &fillMode, jni_bool, 1);
#endif

    setGraphicObjectProperty(destIdentifier, __GO_BACKGROUND__, &background, jni_int, 1);

    setGraphicObjectProperty(destIdentifier, __GO_MARK_FOREGROUND__, &markForeground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_BACKGROUND__, &markBackground, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_SIZE__, &markSize, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);

    return 0;
}
/*--------------------------------------------------------------------------*/
int cloneFontContext(char* sourceIdentifier, char* destIdentifier)
{
    double dblTmp = 0.0;
    double *pdblTmp = &dblTmp;
    double fontSize;
    int fontColor;
    int fontStyle;
    int fontFractional;
    int iTmp = 0;
    int *piTmp = &iTmp;

    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_COLOR__, jni_int, &piTmp);
    fontColor = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_STYLE__, jni_int, &piTmp);
    fontStyle = iTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_SIZE__, jni_double, &pdblTmp);
    fontSize = dblTmp;
    getGraphicObjectProperty(sourceIdentifier, __GO_FONT_FRACTIONAL__, jni_bool, &piTmp);
    fontFractional = iTmp;

    setGraphicObjectProperty(destIdentifier, __GO_FONT_COLOR__, &fontColor, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_STYLE__, &fontStyle, jni_int, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_SIZE__, &fontSize, jni_double, 1);
    setGraphicObjectProperty(destIdentifier, __GO_FONT_FRACTIONAL__, &fontFractional, jni_bool, 1);

    return 0;
}
