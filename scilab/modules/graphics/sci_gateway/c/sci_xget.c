/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
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
#include "api_scilab.h"
#include "gw_graphics.h"
#include "XsetXgetParameters.h"
#include "GetProperty.h"
#include "Format.h"
#include "Scierror.h"
#include "localization.h"
#include "returnProperty.h"
#include "HandleManagement.h"
#include "CurrentSubwin.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"

#include "getHandleProperty.h"
#include "CurrentFigure.h"
/*--------------------------------------------------------------------------*/
int xgetg(char * str, char * str1, int * len, int  lx0, int lx1);
/*--------------------------------------------------------------------------*/
int sci_xget(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    char* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    char* l3 = NULL;

    int m1 = 0, m2 = 0, n2 = 0, i = 0;
    int one = 1;

    BOOL keyFound = FALSE;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    if (getAllocatedSingleString(pvApiCtx, piAddrl1, &l1))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    /* check if key is valid */
    for (i = 0; i < NUMSETFONC ; i++)
    {
        if (strcmp((l1), KeyTab_[i]) == 0)
        {
            keyFound = TRUE;
            break;
        }
    }

    if (!keyFound)
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, (l1));
        freeAllocatedSingleString(l1);
        return -1;
    }

    if (nbInputArgument(pvApiCtx) == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(l1);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
            freeAllocatedSingleString(l1);
            return 1;
        }

        //CheckScalar
        if (m2 != 1 || n2 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 2);
            freeAllocatedSingleString(l1);
            return 1;
        }

    }

    if (strcmp(l1, "fpf") == 0 || strcmp(l1, "auto clear") == 0)
    {
        int bufl;
        char buf[4096];
        /*     special case for global variables set */
        xgetg((l1), buf, &bufl, m1, bsiz);
        if (allocSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, bufl * one, (const char **)&l3))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            freeAllocatedSingleString(l1);
            return 1;
        }

        strncpy((l3), buf, bufl);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "colormap") == 0)
    {
        int iObjUID = 0;
        // Force figure creation if none exists.
        getOrCreateDefaultSubwin();
        iObjUID = getCurrentFigure();

        get_color_map_property(pvApiCtx, iObjUID);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "mark") == 0)
    {
        int iObjUID = getOrCreateDefaultSubwin();
        int iMarkStyle = 0;
        int* piMarkStyle = &iMarkStyle;
        int iMarkSize = 0;
        int* piMarkSize = &iMarkSize;
        double pdblResult[2];

        getGraphicObjectProperty(iObjUID, __GO_MARK_STYLE__, jni_int, (void**)&piMarkStyle);
        getGraphicObjectProperty(iObjUID, __GO_MARK_SIZE__, jni_int, (void**)&piMarkSize);
        pdblResult[0] = iMarkStyle;
        pdblResult[1] = iMarkSize;

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 2, pdblResult);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "mark size") == 0)
    {
        int iObjUID = getOrCreateDefaultSubwin();
        get_mark_size_property(pvApiCtx, iObjUID);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "line style") == 0)
    {
        get_line_style_property(pvApiCtx, getOrCreateDefaultSubwin());

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "clipping") == 0)
    {
        double *clipBox = NULL;
        int iObjUID = getOrCreateDefaultSubwin();

        getGraphicObjectProperty(iObjUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipBox);

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 4, clipBox);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "font") == 0)
    {
        int iObjUID = getOrCreateDefaultSubwin();
        double dblFontSize = 0;
        double* pdblFontSize = &dblFontSize;
        int iFontStyle = 0;
        int* piFontStyle = &iFontStyle;
        double pdblResult[2];

        getGraphicObjectProperty(iObjUID, __GO_FONT_SIZE__, jni_double, (void **)&pdblFontSize);
        getGraphicObjectProperty(iObjUID, __GO_FONT_STYLE__, jni_int, (void**)&piFontStyle);

        pdblResult[0] = iFontStyle;
        pdblResult[1] = dblFontSize;


        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 2, pdblResult);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "font size") == 0)
    {
        double dblFontSize = 0;
        double* pdblFontSize = &dblFontSize;
        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, jni_double, (void **)&pdblFontSize);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dblFontSize);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "dashes") == 0)
    {
        int iLineStyle = 0;
        int* piLineStyle = &iLineStyle;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, jni_int, (void**)&piLineStyle);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iLineStyle);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "hidden3d") == 0)
    {
        get_hidden_color_property(pvApiCtx, getOrCreateDefaultSubwin());

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "window") == 0 || strcmp(l1, "figure") == 0)
    {
        int iFigureId = 0;
        int* piFigureId = &iFigureId;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_ID__, jni_int, (void**)&piFigureId);
        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iFigureId);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "thickness") == 0)
    {
        get_thickness_property(pvApiCtx, getOrCreateDefaultSubwin());

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "wdim") == 0 || strcmp(l1, "wpdim") == 0)
    {
        int *piFigureSize = NULL;
        double pdblFigureSize[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, jni_int_vector, (void **) &piFigureSize);
        pdblFigureSize[0] = (double) piFigureSize[0];
        pdblFigureSize[1] = (double) piFigureSize[1];

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 2, pdblFigureSize);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "wpos") == 0)
    {
        int *piFigurePosition = NULL;
        double pdblFigurePosition[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, jni_int_vector, (void **) &piFigurePosition);
        pdblFigurePosition[0] = piFigurePosition[0];
        pdblFigurePosition[1] = piFigurePosition[1];

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 2, pdblFigurePosition);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "viewport") == 0)
    {
        int* viewport = NULL;
        double pdblViewport[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, jni_int_vector, (void **)&viewport);
        pdblViewport[0] = viewport[0];
        pdblViewport[1] = viewport[1];

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 2, pdblViewport);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "background") == 0)
    {
        get_background_property(pvApiCtx, getOrCreateDefaultSubwin());

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (  strcmp(l1, "color") == 0
               || strcmp(l1, "foreground") == 0
               || strcmp(l1, "pattern") == 0)
    {
        get_foreground_property(pvApiCtx, getOrCreateDefaultSubwin());

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "lastpattern") == 0)
    {
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNumColors);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "line mode") == 0)
    {
        int iLineMode = 0;
        int* lineMode = &iLineMode;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_MODE__, jni_bool, (void **)&lineMode);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iLineMode);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "white") == 0)
    {
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

        /* White is lqst colormap index + 2 */
        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNumColors + 2);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "wresize") == 0)
    {
        // autoresize property
        int iAutoResize = 0;
        int* piAutoResize =  &iAutoResize;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_AUTORESIZE__, jni_bool, (void **)&piAutoResize);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iAutoResize);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "clipgrf") == 0)
    {
        /* clip_state : 0 = off, 1 = on */
        int iClipState = 0;
        int* piClipState = &iClipState;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, (void**)&piClipState);

        createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iClipState);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else if (strcmp(l1, "clipoff") == 0)
    {
        int iClipState = 0;
        int* piClipState = &iClipState;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, (void**)&piClipState);

        /* clip_state : 0 = off, 1 = on */
        if (iClipState == 0)
        {
            createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1);
        }
        else
        {
            createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0);
        }
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, (l1));
        freeAllocatedSingleString(l1);
        return -1;
    }

    freeAllocatedSingleString(l1);
    return 0;
}
/*--------------------------------------------------------------------------*/
int xgetg(char * str, char * str1, int * len, int  lx0, int lx1)
{
    if (strcmp(str, "fpf") == 0)
    {
        strncpy(str1, getFPF(), 32);
        *len = (int) strlen(str1);
    }
    else if (strcmp(str, "auto clear") == 0)
    {
        int iAutoClear = 0;
        int* piAutoClear = &iAutoClear;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, jni_bool, (void **)&piAutoClear);
        if (iAutoClear == 1)
        {
            strncpy(str1, "on", 2);
            *len = 2;
        }
        else
        {
            strncpy(str1, "off", 3);
            *len = 3;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
