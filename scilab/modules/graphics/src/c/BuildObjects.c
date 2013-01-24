/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
 *    This file contains all functions used to BUILD new objects :
 - allocating memory
 - setting default value
 - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "BuildObjects.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "CloneObjects.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "CurrentFigure.h"
#include "FigureList.h"
#include "localization.h"
#include "Interaction.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"

#include "MALLOC.h"             /* MALLOC */
#include "Scierror.h"

#include "Format.h"             // computeDefaultTicsLabels

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "FigureModel.h"
#include "AxesModel.h"

/**
 * If a current figure exists : return it
 * Otherwise create a new one.
 *
 * This method also alocate an axe object.
 *
 * After this call: the current figure the current axes and the current subwin
 * are set to the appropriate values.
 *
 * @return a reference to the current figure.
 */
GRAPHICS_IMPEXP char * createNewFigureWithAxes()
{
    int iID = 0;
    char *pFigureUID = NULL;
    int* axesSize = NULL;

    pFigureUID = cloneGraphicObject(getFigureModel());

    /*
     * Clone the default menus
     */
    cloneMenus((char*)getFigureModel(), pFigureUID);

    setGraphicObjectProperty(pFigureUID, __GO_ID__, &iID, jni_int, 1);

    /*
     * Clone the default axes
     */
    cloneAxesModel(pFigureUID);
    setCurrentFigure(pFigureUID);
    /*
     * Force axes size after window creation ( Java )
     */
    getGraphicObjectProperty(getFigureModel(), __GO_AXES_SIZE__, jni_int_vector, (void **)&axesSize);
    setGraphicObjectProperty(pFigureUID, __GO_AXES_SIZE__, axesSize, jni_int_vector, 2);

    // return the reference to the current figure
    releaseGraphicObjectProperty(__GO_PARENT__, pFigureUID, jni_string, 1);
    return (char*)getCurrentFigure();
}

/**
 * Clone a new Axes object using the Axes model which is then
 * attached to the newly created Figure.
 *
 * After this call: tthe current axes and the current subwin are set to the
 * appropriate values.
 */
GRAPHICS_IMPEXP void cloneAxesModel(char const* pstFigureUID)
{
    char *pAxesUID = cloneGraphicObject(getAxesModel());

    /* Clone the Axes model's labels and attach them to the newly created Axes */
    ConstructLabel(pAxesUID, "", 1);
    ConstructLabel(pAxesUID, "", 2);
    ConstructLabel(pAxesUID, "", 3);
    ConstructLabel(pAxesUID, "", 4);

    /* Sets the parent-child relationship within the MVC */
    setGraphicObjectRelationship(pstFigureUID, pAxesUID);

    /* Sets the newly created Axes as the Figure's current selected child */
    setGraphicObjectProperty(pstFigureUID, __GO_SELECTED_CHILD__, pAxesUID, jni_string, 1);

    // Set new axes as default too.
    setCurrentObject(pAxesUID);
    setCurrentSubWin(pAxesUID);

    releaseGraphicObjectProperty(__GO_PARENT__, pAxesUID, jni_string, 1);
}

GRAPHICS_IMPEXP void cloneMenus(char * pModelUID, char * pCloneUID)
{
    int iNbChildren = 0;
    int *piNbChildren = &iNbChildren;
    int iChild = 0;
    char *pChildUID = NULL;
    char **pChildren = NULL;
    int iChildType = -1;
    int *piChildType = &iChildType;

    getGraphicObjectProperty(pModelUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piNbChildren);
    getGraphicObjectProperty(pModelUID, __GO_CHILDREN__, jni_string_vector, (void **)&pChildren);
    for (iChild = iNbChildren - 1; iChild >= 0; iChild--)
    {
        getGraphicObjectProperty(pChildren[iChild], __GO_TYPE__, jni_int, (void **)&piChildType);
        if (iChildType == __GO_UIMENU__)
        {
            pChildUID = cloneGraphicObject(pChildren[iChild]);

            setGraphicObjectRelationship(pCloneUID, pChildUID);
            cloneMenus(pChildren[iChild], pChildUID);

            releaseGraphicObjectProperty(__GO_PARENT__, pChildUID, jni_string, 1);
        }
    }
    releaseGraphicObjectProperty(__GO_CHILDREN__, pChildren, jni_string_vector, iNbChildren);
}

/**
 * If a current subwin exists: return it
 * Otherwise create a new figure with JoGLView.
 **/
GRAPHICS_IMPEXP char const* getOrCreateDefaultSubwin(void)
{
    char const* pSubWinUID = getCurrentSubWin();

    if (pSubWinUID == NULL)
    {
        createNewFigureWithAxes();
        // the current figure,
        pSubWinUID = getCurrentSubWin();
    }

    return pSubWinUID;
}

/*-----------------------------------------------------------------------------*/

/**ConstructSubWin
 * This function creates the Subwindow (the Axes) and the elementary structures.
 * The update of color properties (foreground, background, etc.)
 * according to the assigned parent Figure's colormap is not implemented yet.
 * To be implemented.
 *
 * @return a reference to the current object (will be invalidated on current object modification)
 */
char const* ConstructSubWin(char const* pparentfigureUID)
{
    int parentType = -1;
    int *piParentType = &parentType;
    char *pCloneUID = NULL;
    char const* paxesmdlUID = getAxesModel();

    getGraphicObjectProperty(pparentfigureUID, __GO_TYPE__, jni_int, (void**) &piParentType);

    if (parentType != __GO_FIGURE__)
    {
        Scierror(999, _("The parent has to be a FIGURE\n"));
        return (char *)NULL;
    }

    pCloneUID = cloneGraphicObject(paxesmdlUID);

    /* Clone the Axes model's labels and attach them to the newly created Axes */
    ConstructLabel(pCloneUID, "", 1);
    ConstructLabel(pCloneUID, "", 2);
    ConstructLabel(pCloneUID, "", 3);
    ConstructLabel(pCloneUID, "", 4);

    setGraphicObjectRelationship(pparentfigureUID, pCloneUID);

    setCurrentObject(pCloneUID);
    sciSetSelectedSubWin(pCloneUID);
    setCurrentSubWin(pCloneUID);

    releaseGraphicObjectProperty(__GO_PARENT__, pCloneUID, jni_string, 1);

    return getCurrentObject();
}

/**
 * Creates a new text object. However the object is not added in the handle list.
 * Its graphic and font contexts are initialized.
 * This function is to be used with objects including a text object.
 */
char * allocateText(char * pparentsubwinUID,
                    char * * text,
                    int nbRow,
                    int nbCol,
                    double x,
                    double y,
                    BOOL autoSize,
                    double userSize[2],
                    int centerPos, int *foreground, int *background, BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align)
{
    char * pobjUID = NULL;
    int textDimensions[2];
    int visible = 0;
    int *piVisible = &visible;
    int clipRegionSet;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;

    double *clipRegion = NULL;
    double position[3];
    double setUserSize[2];

    pobjUID = (char *)createGraphicObject(__GO_TEXT__);

    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, piVisible, jni_bool, 1);

    /* Clipping: to be checked for consistency */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, piClipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, piClipState, jni_int, 1);

    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(text, nbRow, nbCol);

    /* Allocates the String array */
    textDimensions[0] = nbRow;
    textDimensions[1] = nbCol;

    setGraphicObjectProperty(pobjUID, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);

    setGraphicObjectProperty(pobjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, nbRow * nbCol);

    position[0] = x;
    position[1] = y;
    position[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_POSITION__, position, jni_double_vector, 3);

    setGraphicObjectProperty(pobjUID, __GO_TEXT_BOX_MODE__, &centerPos, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_AUTO_DIMENSIONING__, &autoSize, jni_bool, 1);

    /* userSize must be specified if the size is given by the user */
    /* or the user specified a rectangle */
    if (!autoSize || centerPos)
    {
        setUserSize[0] = userSize[0];
        setUserSize[1] = userSize[1];
    }
    else
    {
        setUserSize[0] = 0.0;
        setUserSize[1] = 0.0;
    }

    setGraphicObjectProperty(pobjUID, __GO_TEXT_BOX__, setUserSize, jni_double_vector, 2);

    /* Required to get the correct MVC value from the sciTextAlignment enum */
    align = align - 1;

    /* Set alignment to left if its value is incorrect */
    if (align < 0 || align > 2)
    {
        align = 0;
    }

    setGraphicObjectProperty(pobjUID, __GO_ALIGNMENT__, &align, jni_int, 1);

    cloneGraphicContext(pparentsubwinUID, pobjUID);

    cloneFontContext(pparentsubwinUID, pobjUID);

    setGraphicObjectProperty(pobjUID, __GO_BOX__, &isboxed, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if (foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if (background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, foreground, jni_int, 1);
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    return pobjUID;
}

/**ConstructText
 * This function creates the parents window (manager) and the elementaries structures
 * @param  char *pparentsubwinUID : parent subwin UID
 * @param  char * text[] : intial text matrix string.
 * @param  int nbCol : the number column of the text
 * @param  int nbRow : the number of row of the text
 * @return  : object UID if ok , NULL if not
 */
char * ConstructText(char * pparentsubwinUID, char **text, int nbRow, int nbCol, double x,
                     double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background,
                     BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align)
{
    int parentType = -1;
    int *piParentType = &parentType;
    char *pobjUID = NULL;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = allocateText(pparentsubwinUID, text, nbRow, nbCol, x, y,
                           autoSize, userSize, centerPos, foreground, background, isboxed, isline, isfilled, align);

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);
    setCurrentObject(pobjUID);
    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);

    return (char*)getCurrentObject();
}

/**ConstructLegend
 * This function creates a Legend structure
 * @param  char *pparentsubwinUID : parent subwin UID
 * @param  char ** text : initial text matrix string array
 * @param long long tabofhandles[] : the array of polyline handles
 * @param int nblegends : the number of legend items
 * @return : object UID if ok , NULL if not
 */
char * ConstructLegend(char * pparentsubwinUID, char **text, long long tabofhandles[], int nblegends)
{
    char *pobjUID = NULL;

    int i = 0;
    int iLegendPresent = 0;
    int *piLegendPresent = &iLegendPresent;
    int iVisible = 0;
    int *piVisible = &iVisible;
    int textDimensions[2];
    int fillMode = 0;
    int legendLocation = 0;

    int clipRegionSet = 0;
    int clipState = 0;

    double *clipRegion = NULL;
    double position[2];

    char **lineIDS = NULL;
    int parentType = -1;
    int *piParentType = &parentType;

    /* Check beforehand whether a Legend object is already present */
    getGraphicObjectProperty(pparentsubwinUID, __GO_HAS_LEGEND_CHILD__, jni_bool, (void **)&piLegendPresent);

    if (iLegendPresent)
    {
        /* Delete it (one Legend object allowed at most) */
        char *legendChildID;

        getGraphicObjectProperty(pparentsubwinUID, __GO_LEGEND_CHILD__, jni_string, (void **)&legendChildID);

        deleteGraphicObject(legendChildID);
        releaseGraphicObjectProperty(__GO_LEGEND_CHILD__, legendChildID, jni_string, 1);
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = (char *)createGraphicObject(__GO_LEGEND__);

    /* Required to initialize the default contour and font properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &iVisible, jni_bool, 1);

    lineIDS = (char **)MALLOC(nblegends * sizeof(char *));
    if (lineIDS == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "ConstructLegend");
        deleteGraphicObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return (char *)NULL;
    }

    textDimensions[0] = nblegends;
    textDimensions[1] = 1;

    setGraphicObjectProperty(pobjUID, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);
    setGraphicObjectProperty(pobjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, nblegends);

    for (i = 0; i < nblegends; i++)
    {
        char * tmpObjUID;

        tmpObjUID = (char*)getObjectFromHandle((long)tabofhandles[i]);

        /*
         * Links are ordered from most recent to least recent,
         * as their referred-to Polylines in the latter's parent Compound object.
         */
        lineIDS[nblegends - i - 1] = tmpObjUID;
    }

    setGraphicObjectProperty(pobjUID, __GO_LINKS__, lineIDS, jni_string_vector, nblegends);

    /*
     * Do not release tmpObjUIDs (eg lineIDS content) as getObjectFromHandle pass data by reference.
     */
    FREE(lineIDS);

    position[0] = 0.0;
    position[1] = 0.0;
    setGraphicObjectProperty(pobjUID, __GO_POSITION__, position, jni_double_vector, 2);

    /* 9: LOWER_CAPTION */
    legendLocation = 9;
    setGraphicObjectProperty(pobjUID, __GO_LEGEND_LOCATION__, &legendLocation, jni_int, 1);

    /* Clipping: to be checked for consistency */
    clipRegionSet = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    /* 0: OFF */
    clipState = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    /* NEW :  used to draw the line and marks of the curve F.Leray 21.01.05 */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    cloneFontContext(pparentsubwinUID, pobjUID);

    fillMode = TRUE;
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);

    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);
    setCurrentObject(pobjUID);
    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);

    return (char*)getCurrentObject();
}

/*---------------------------------------------------------------------------------*/
/**
 * Create a polyline but does not add it to Scilab hierarchy
 */
char * allocatePolyline(char * pparentsubwinUID, double *pvecx, double *pvecy, double *pvecz,
                        int closed, int n1, int plot, int *foreground, int *background,
                        int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    char *pobjUID = NULL;
    int i = 0;
    BOOL result = FALSE;
    char *type = NULL;
    char *polylineID = NULL;
    double barWidth = 0.;
    double arrowSizeFactor = 0.;
    double *clipRegion = NULL;
    double *dataVector = NULL;
    int clipState = 0;
    int *piClipState = &clipState;
    int lineClosed = 0;
    int numElementsArray[2];
    int visible = 0;
    int *piVisible = &visible;
    int zCoordinatesSet = 0;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;

    pobjUID = (char *) createGraphicObject(__GO_POLYLINE__);
    polylineID = (char *) createDataObject(pobjUID, __GO_POLYLINE__);

    if (polylineID == NULL)
    {
        deleteGraphicObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return NULL;
    }

    barWidth = 0.0;
    setGraphicObjectProperty(pobjUID, __GO_BAR_WIDTH__, &barWidth, jni_double, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* Clip state and region */
    /* To be checked for consistency */

    /*
     * releaseGraphicObjectProperty for any property passed by reference only
     */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    arrowSizeFactor = 1.0;
    setGraphicObjectProperty(pobjUID, __GO_ARROW_SIZE_FACTOR__, &arrowSizeFactor, jni_double, 1);

    /*
     * First element: number of gons (always 1 for a Polyline)
     * Second one: number of vertices composing the Polyline
     */
    numElementsArray[0] = 1;
    numElementsArray[1] = n1;

    /* Data */
    if (n1 != 0)
    {
        /*
         * Sets the number of elements (vertices composing the polyline) and allocates the coordinates array
         * The FALSE value is used to identify a failed memory allocation for now.
         */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
	    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
            return NULL;
        }

        dataVector = MALLOC(3 * n1 * sizeof(double));

        if (dataVector == NULL)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
	    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
            return NULL;
        }

        if ((pvecx != (double *)NULL) && (pvecy != (double *)NULL))
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = pvecx[i];
                dataVector[n1 + i] = pvecy[i];
            }
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = 0.0;
                dataVector[n1 + i] = 0.0;
            }
        }

        /**DJ.Abdemouche 2003**/
        if (pvecz == NULL)
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2 * n1 + i] = 0.0;
            }

            zCoordinatesSet = 0;
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2 * n1 + i] = pvecz[i];
            }

            zCoordinatesSet = 1;
        }

        /*
         * We could probably do without the dataVector copy by individually setting
         * x, y or z coordinates, and initializing coordinates to 0 during allocation
         * to ensure consistency
         */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, dataVector, jni_double, n1);

        FREE(dataVector);

        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_double, n1);
    }
    else
    {
        /* 0 points */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
	    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
            return NULL;
        }
    }

    if (closed > 0)
    {
        lineClosed = 1;
    }
    else
    {
        lineClosed = 0;
    }

    setGraphicObjectProperty(pobjUID, __GO_CLOSED__, &lineClosed, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_POLYLINE_STYLE__, &plot, jni_int, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the default values (those of the parent Axes).
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* colors and marks setting */
    setGraphicObjectProperty(pobjUID, __GO_MARK_MODE__, &ismark, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    /* shading interpolation vector and mode */
    setGraphicObjectProperty(pobjUID, __GO_INTERP_COLOR_MODE__, &isinterpshaded, jni_bool, 1);

    if (foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if (background != NULL)
    {
        if (isinterpshaded == TRUE)
        {
            /* 3 or 4 values to store */

            setGraphicObjectProperty(pobjUID, __GO_INTERP_COLOR_VECTOR__, background, jni_int_vector, n1);
        }
        else
        {
            setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);
        }
    }

    if (mark_style != NULL)
    {
        /* This does use the MVC */
        setGraphicObjectProperty(pobjUID, __GO_MARK_STYLE__, mark_style, jni_int, 1);
    }

    if (mark_foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_MARK_FOREGROUND__, mark_foreground, jni_int, 1);
    }

    if (mark_background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_MARK_BACKGROUND__, mark_background, jni_int, 1);
    }

    visible = 1;
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    return pobjUID;
}

/*---------------------------------------------------------------------------------*/
/**ConstructPolyline
 * This function creates  Polyline 2d structure
 */
char * ConstructPolyline(char * pparentsubwinUID, double *pvecx, double *pvecy, double *pvecz,
                         int closed, int n1, int plot, int *foreground, int *background,
                         int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    char * pobjUID = allocatePolyline(pparentsubwinUID, pvecx, pvecy, pvecz, closed, n1, plot,
                                      foreground, background, mark_style, mark_foreground, mark_background,
                                      isline, isfilled, ismark, isinterpshaded);
    return pobjUID;
}

/**ConstructArc
 * This function creates an Arc structure
 */
char * ConstructArc(char * pparentsubwinUID, double x, double y,
                    double height, double width, double alphabegin, double alphaend, int *foreground, int *background, BOOL isfilled, BOOL isline)
{
    char *pobjUID = NULL;
    int type = -1;
    int *piType = &type;
    double upperLeftPoint[3];
    double *clipRegion = NULL;
    int visible = 0;
    int *piVisible = &visible;
    int arcDrawingMethod = 0;
    int *piArcDrawingMethod = &arcDrawingMethod;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = (char *)createGraphicObject(__GO_ARC__);

    /*
     * Sets the arc's parent in order to initialize the former's Contoured properties
     * with the latter's values (cloneGraphicContext call below)
     */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    upperLeftPoint[0] = x;
    upperLeftPoint[1] = y;
    upperLeftPoint[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

    setGraphicObjectProperty(pobjUID, __GO_HEIGHT__, &height, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_WIDTH__, &width, jni_double, 1);

    setGraphicObjectProperty(pobjUID, __GO_START_ANGLE__, &alphabegin, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_END_ANGLE__, &alphaend, jni_double, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_ARC_DRAWING_METHOD__, jni_int, (void **)&piArcDrawingMethod);

    setGraphicObjectProperty(pobjUID, __GO_ARC_DRAWING_METHOD__, &arcDrawingMethod, jni_int, 1);

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the parent Axes' values.
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* Contour settings */
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if (foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if (background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    /*
     * Sets the Axes as the arc's parent and adds the arc to
     * its parent's list of children.
     */
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}

/**ConstructRectangle
 * This function creates Rectangle structure and only this to destroy all sons use DelGraphicsSon
 */
char * ConstructRectangle(char * pparentsubwinUID, double x, double y,
                          double height, double width, int *foreground, int *background, int isfilled, int isline)
{
    char *pobjUID = NULL;
    char *type = NULL;
    double upperLeftPoint[3];
    double *clipRegion = NULL;
    int visible = 0;
    int *piVisible = &visible;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int iMarkMode = 0;
    int *piMarkMode = &iMarkMode;

    if (height < 0.0 || width < 0.0)
    {
        Scierror(999, _("Width and height must be positive.\n"));
        return NULL;
    }

    pobjUID = (char *)createGraphicObject(__GO_RECTANGLE__);

    /*
     * Sets the rectangle's parent in order to initialize the former's Contoured properties
     * with the latter's values (cloneGraphicContext call below)
     */
    //setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    upperLeftPoint[0] = x;
    upperLeftPoint[1] = y;
    upperLeftPoint[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

    setGraphicObjectProperty(pobjUID, __GO_HEIGHT__, &height, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_WIDTH__, &width, jni_double, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* Clip state and region */
    /* To be checked for consistency */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_MARK_MODE__, jni_bool, (void **)&piMarkMode);
    setGraphicObjectProperty(pobjUID, __GO_MARK_MODE__, &iMarkMode, jni_bool, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the default values (those of the parent Axes).
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* Contour settings */
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if (foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if (background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);
    }

    /* Parent reset to the null object */
    //setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    /*
     * Sets the Axes as the rectangle's parent and adds the rectangle to
     * its parent's list of children.
     */
    //setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}

/**ConstructSurface
 * This function creates Surface Structure
 */
char *ConstructSurface(char *pparentsubwinUID, sciTypeOf3D typeof3d,
                       double *pvecx, double *pvecy, double *pvecz, double *zcol,
                       int izcol, int dimzx, int dimzy,
                       int *flag, double *ebox, int flagcolor, int *isfac, int *m1, int *n1, int *m2, int *n2, int *m3, int *n3, int *m3n, int *n3n)
{
    char *pobjUID = NULL;
    int parentType = -1;
    int *piParentType = &parentType;
    int const surfaceTypes[2] = { __GO_PLOT3D__, __GO_FAC3D__ };

    double *clipRegion = NULL;

    int nx = 0, ny = 0, nz = 0, nc = 0;
    int result = 0;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int visible = 0;
    int *piVisible = &visible;
    int cdataMapping = 0;
    int hiddenColor = 0;
    int *piHiddenColor = &hiddenColor;
    int surfaceMode = 0;

    /* To be modified: the MVC does not allow Plot3d objects with color data yet */
    if (typeof3d == SCI_PLOT3D)
    {
        nx = dimzx;
        ny = dimzy;
        nz = dimzx * dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzx * dimzy */
            nc = nz;
        }
        else if (flagcolor == 3)
        {
            /*
             * one color per edge: nc = 4* dimzx * dimzy ??????
             * 3 or 4 vertices are needed: I think we take 4 to have enough allocated memory
             */
            nc = nz * 4;
        }
        /* made by Djalel : comes from the genfac3d case */
        else
        {
            nc = 0;
        }
    }
    /* DJ.A 2003 */
    else
    {
        /* case SCI_FAC3D */
        nx = dimzx * dimzy;
        ny = dimzx * dimzy;
        nz = dimzx * dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzy */
            nc = dimzy;
        }
        else if (flagcolor == 3)
        {
            /* one color per edge: nc = dimzx * dimzy */
            nc = nz;
        }
        else
        {
            nc = 0;
        }
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return NULL;
    }

    pobjUID = (char *)createGraphicObject(surfaceTypes[*isfac]);
    createDataObject(pobjUID, surfaceTypes[*isfac]);

    /* Clip state and region */
    /* To be checked for consistency */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* Visibility */
    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    setGraphicObjectProperty(pobjUID, __GO_COLOR_FLAG__, &flagcolor, jni_int, 1);

    /* Direct mode enabled as default */
    cdataMapping = 1;

    /* Only for Fac3D */
    setGraphicObjectProperty(pobjUID, __GO_DATA_MAPPING__, &cdataMapping, jni_int, 1);

    setGraphicObjectProperty(pobjUID, __GO_COLOR_MODE__, &flag[0], jni_int, 1);

    /* Plot3d case */
    if (!*isfac)
    {
        int gridSize[4];

        gridSize[0] = *m1;
        gridSize[1] = *n1;
        gridSize[2] = *m2;
        gridSize[3] = *n2;

        /* Allocates the coordinates arrays */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    }
    /* Fac3d case */
    else
    {
        int numElementsArray[3];

        /*
         * First element: number of n-gons
         * Second element: number of vertices per n-gon
         * Third element: number of input colors
         */
        numElementsArray[0] = dimzy;
        numElementsArray[1] = dimzx;
        numElementsArray[2] = nc;

        /* Allocates the coordinates and color arrays */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);
    }

    if (result == 0)
    {
        deleteGraphicObject(pobjUID);
        deleteDataObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return NULL;
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, nx);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, ny);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nz);

    /* Add the color matrix dimensions as a property ? */
    if (nc > 0)
    {
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, zcol, jni_double_vector, nc);
    }

    /*-------END Replaced by: --------*/

    getGraphicObjectProperty(pparentsubwinUID, __GO_HIDDEN_COLOR__, jni_int, (void **)&piHiddenColor);
    setGraphicObjectProperty(pobjUID, __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);

    /*
     * surfaceMode set to "on", was previously done by InitGraphicContext, by setting
     * the graphic context's line_mode to on, which stood for the surface_mode.
     */
    surfaceMode = 1;

    setGraphicObjectProperty(pobjUID, __GO_SURFACE_MODE__, &surfaceMode, jni_bool, 1);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
    // Here we init old 'graphicContext' by cloning it from parent.
    cloneGraphicContext(pparentsubwinUID, pobjUID);
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}

/********************** 14/05/2002 *****
 **ConstructGrayplot
 * This function is used to build Grayplot and Matplot objects.
 * It would probably be better to put the code related to Matplot objects
 * in a separate build function, as it would avoid having to perform several tests
 * on the type parameter. This is done so because Matplot objects were previously
 * internally represented by sciGrayplot structures.
 */
char *ConstructGrayplot(char *pparentsubwinUID, double *pvecx, double *pvecy, double *pvecz, int n1, int n2, int type)
{
    char *pobjUID = NULL;

    int const objectTypes[3] = { __GO_GRAYPLOT__, __GO_MATPLOT__, __GO_MATPLOT__ };

    int typeParent = -1;
    int *piTypeParent = &typeParent;

    char *grayplotID = NULL;
    int result = 0;
    int dataMapping = 0;
    int gridSize[4];

    int parentVisible = 0;
    int *piParentVisible = &parentVisible;
    double *clipRegion = NULL;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int numElements = 0;

    double pdblScale[2];

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piTypeParent);

    if (typeParent != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = (char *)createGraphicObject(objectTypes[type]);
    grayplotID = (char *)createDataObject(pobjUID, objectTypes[type]);

    if (grayplotID == NULL)
    {
        deleteGraphicObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return NULL;
    }

    /* 0: scaled; only used for Grayplot */
    if (type == 0)
    {
        dataMapping = 0;
        setGraphicObjectProperty(pobjUID, __GO_DATA_MAPPING__, &dataMapping, jni_int, 1);
    }

    /* The x and y vectors are column ones */

    /*
     * For the Grayplot object, the number of rows and columns respectively
     * correspond to the grid's x and y dimensions whereas for Matplot objects,
     * they respectively correspond to the grid's y and x dimensions.
     */
    if (type == 0)
    {
        gridSize[0] = n1;
        gridSize[1] = 1;
        gridSize[2] = n2;
        gridSize[3] = 1;
    }
    else
    {
        gridSize[0] = n2;
        gridSize[1] = 1;
        gridSize[2] = n1;
        gridSize[3] = 1;
    }

    /* Only for Matplot1 objects */
    if (type == 2)
    {
        setGraphicObjectProperty(pobjUID, __GO_MATPLOT_TRANSLATE__, pvecx, jni_double_vector, 2);
        pdblScale[0] = (pvecx[2] - pvecx[0]) / (n2 - 1.0);
        pdblScale[1] = (pvecx[3] - pvecx[1]) / (n1 - 1.0);
        setGraphicObjectProperty(pobjUID, __GO_MATPLOT_SCALE__, pdblScale, jni_double_vector, 2);
    }

    /* Allocates the coordinates arrays */
    result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    if (result == 0)
    {
        deleteGraphicObject(pobjUID);
        deleteDataObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return NULL;
    }

    /* Only for Grayplot objects, for Matplot objects, x and y coordinates are automatically computed */
    if (type == 0)
    {
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, n1);
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, n2);
    }

    if (type == 0)
    {
        numElements = n1 * n2;
    }
    else
    {
        numElements = (n1 - 1) * (n2 - 1);
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, numElements);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piParentVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* Initializes the default Contour values */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    return pobjUID;
}

/**ConstructAxis
 * This function creates an Axis object
 * @author Djalel ABDEMOUCHE
 * @see sciSetCurrentObj
 *
 */
char *ConstructAxis(char *pparentsubwinUID, char dir, char tics, double *vx,
                    int nx, double *vy, int ny, char **str, int subint, char *format,
                    int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels)
{
    int parentType = -1;
    int *piParentType = &parentType;
    char *pobjUID = NULL;
    int i = 0;
    int clipRegionSet = 0;
    int clipState = 0;
    int ticksDirection = 0;
    int ticksStyle = 0;
    double *clipRegion = NULL;
    double doubleFontSize = 0.;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = (char *)createGraphicObject(__GO_AXIS__);

    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    /* Clipping: to be checked for consistency */
    clipRegionSet = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    /* 0: OFF */
    clipState = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* The ticks style and direction MVC properties are Integers */
    if (dir == 'u')
    {
        ticksDirection = 0;
    }
    else if (dir == 'd')
    {
        ticksDirection = 1;
    }
    else if (dir == 'l')
    {
        ticksDirection = 2;
    }
    else if (dir == 'r')
    {
        ticksDirection = 3;
    }
    else
    {
        ticksDirection = 0;
    }

    if (tics == 'v')
    {
        ticksStyle = 0;
    }
    else if (tics == 'r')
    {
        ticksStyle = 1;
    }
    else if (tics == 'i')
    {
        ticksStyle = 2;
    }
    else
    {
        ticksStyle = 0;
    }

    setGraphicObjectProperty(pobjUID, __GO_TICKS_DIRECTION__, &ticksDirection, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_TICKS_STYLE__, &ticksStyle, jni_int, 1);

    setGraphicObjectProperty(pobjUID, __GO_X_TICKS_COORDS__, vx, jni_double_vector, nx);
    setGraphicObjectProperty(pobjUID, __GO_Y_TICKS_COORDS__, vy, jni_double_vector, ny);

    /* FORMATN must be set before Labels are computed. */
    if (format != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_FORMATN__, format, jni_string, 1);
    }

    /*
     * Labels are computed automatically depending on the ticks coordinates.
     * The computation is performed by a C function which has been adapted
     * to the MVC (property get calls) and was previously done in the
     * tics labels property get C function.
     * It should be done (or called) directly in the Java part of the model
     * for the sake of efficiency.
     * To be modified
     */
    if (str == NULL)
    {
        char **matData;
        StringMatrix *tics_labels;

        tics_labels = computeDefaultTicsLabels(pobjUID);

        if (tics_labels == NULL)
        {
            deleteGraphicObject(pobjUID);
	    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
            return (char *)NULL;
        }

        matData = getStrMatData(tics_labels);

        /*
         * The labels vector size must be computed using the matrix's dimensions.
         * To be modified when the labels computation is moved to the Model.
         */
        setGraphicObjectProperty(pobjUID, __GO_TICKS_LABELS__, matData, jni_string_vector, tics_labels->nbCol * tics_labels->nbRow);

        deleteMatrix(tics_labels);
    }
    else
    {
        /*
         * Labels are set using the str argument; the previous code tested whether each element of the
         * str array was null and set the corresponding Axis' element to NULL, though there was no
         * apparent reason to do so. This is still checked, but now aborts building the Axis.
         */

        if (nb_tics_labels == -1)
        {
            Scierror(999, _("Impossible case when building axis\n"));
            deleteGraphicObject(pobjUID);
            releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
            return NULL;
        }

        for (i = 0; i < nb_tics_labels; i++)
        {
            if (str[i] == NULL)
            {
                deleteGraphicObject(pobjUID);
                releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
                return NULL;
            }
        }

        setGraphicObjectProperty(pobjUID, __GO_TICKS_LABELS__, str, jni_string_vector, nb_tics_labels);
    }

    setGraphicObjectProperty(pobjUID, __GO_SUBTICKS__, &subint, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_TICKS_SEGMENT__, &seg, jni_bool, 1);

    /* Initializes the default Contour values */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* Initializes the default Font values */
    cloneFontContext(pparentsubwinUID, pobjUID);

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    doubleFontSize = (double)fontsize;

    setGraphicObjectProperty(pobjUID, __GO_FONT_SIZE__, &doubleFontSize, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_FONT_COLOR__, &textcolor, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_TICKS_COLOR__, &ticscolor, jni_int, 1);

    return pobjUID;
}

/********************** 21/05/2002 *****
 **ConstructFec
 * This function creates Fec
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
char *ConstructFec(char *pparentsubwinUID, double *pvecx, double *pvecy, double *pnoeud,
                   double *pfun, int Nnode, int Ntr, double *zminmax, int *colminmax, int *colout, BOOL with_mesh)
{
    char *pobjUID = NULL;
    char *fecId = NULL;
    int result = 0;

    int parentType = -1;
    int *piParentType = &parentType;

    int parentVisible = 0;
    int *piParentVisible = &parentVisible;

    double *clipRegion = NULL;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int iClipState = 0;
    int *piClipState = &iClipState;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *)NULL;
    }

    pobjUID = createGraphicObject(__GO_FEC__);
    fecId = (char *)createDataObject(pobjUID, __GO_FEC__);

    if (fecId == NULL)
    {
        deleteGraphicObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return (char *)NULL;
    }

    /* Allocates the coordinates array */
    result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES__, &Nnode, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(pobjUID);
        deleteDataObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return (char *)NULL;
    }

    /* Allocates the triangle indices and values array */
    result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_INDICES__, &Ntr, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(pobjUID);
        deleteDataObject(pobjUID);
        releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
        return (char *)NULL;
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, Nnode);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, Nnode);

    /* Fec-specific property: triangle indices plus special values (triangle number and flag) */
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_FEC_TRIANGLES__, pnoeud, jni_double_vector, Ntr);

    /* Function values */
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_VALUES__, pfun, jni_double_vector, Nnode);

    setGraphicObjectProperty(pobjUID, __GO_Z_BOUNDS__, zminmax, jni_double_vector, 2);
    setGraphicObjectProperty(pobjUID, __GO_COLOR_RANGE__, colminmax, jni_int_vector, 2);
    setGraphicObjectProperty(pobjUID, __GO_OUTSIDE_COLOR__, colout, jni_int_vector, 2);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piParentVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &iClipState, jni_int, 1);

    /* Initializes the default Contour values */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* line mode is set using with_mesh */
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &with_mesh, jni_bool, 1);

    return pobjUID;
}

/**ConstructSegs
 * This function creates Segments
 * It is used to create and initialize the data of both the Champ and Segs MVC objects.
 * @author Djalel.ABDEMOUCHE
 * @version 0.1
 * @see sciSetCurrentObj
 */
char *ConstructSegs(char *pparentsubwinUID, int type,
                    double *vx, double *vy, double *vz,
                    int Nbr1, int Nbr2, int Nbr3, double *vfx, double *vfy, int flag, int *style, double arsize, int colored, int typeofchamp)
{
    char *pobjUID = NULL;

    int visible = 0;
    int *piVisible = &visible;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int numberArrows = 0;
    int dimensions[2];
    int i = 0;

    double *clipRegion = NULL;
    double *arrowCoords = NULL;

    if (type == 0)
    {
        pobjUID = createGraphicObject(__GO_SEGS__);
    }
    else if (type == 1)
    {
        pobjUID = createGraphicObject(__GO_CHAMP__);
    }
    else
    {
        return (char *)NULL;
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* this must be done prior to the call of Set Clipping property to know */
    /* if the clip_state has been set */

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    if (type == 1)
    {
        numberArrows = Nbr1 * Nbr2;
    }
    else
    {
        /* Segs: Nbr1/2 arrows, Nbr1 is the number of endpoints */
        numberArrows = Nbr1 / 2;
    }

    /* Triggers the creation of the Arrow objects part of Champ or Segs */
    setGraphicObjectProperty(pobjUID, __GO_NUMBER_ARROWS__, &numberArrows, jni_int, 1);

    /* Champ property only */
    if (type == 1)
    {
        dimensions[0] = Nbr1;
        dimensions[1] = Nbr2;

        setGraphicObjectProperty(pobjUID, __GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);
    }

    arrowCoords = (double *)MALLOC(3 * numberArrows * sizeof(double));

    if (arrowCoords == NULL)
    {
        deleteGraphicObject(pobjUID);
	releaseGraphicObjectProperty(__GO_SEGS__, pobjUID, jni_string, 1);
        return (char *)NULL;
    }

    setGraphicObjectProperty(pobjUID, __GO_ARROW_SIZE__, &arsize, jni_double, 1);

    /* Type 0 corresponds to a SEGS object */
    if (type == 0)
    {
        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3 * i] = vx[2 * i];
            arrowCoords[3 * i + 1] = vy[2 * i];

            if (vz != NULL)
            {
                arrowCoords[3 * i + 2] = vz[2 * i];
            }
            else
            {
                arrowCoords[3 * i + 2] = 0.0;
            }
        }

        setGraphicObjectProperty(pobjUID, __GO_BASE__, arrowCoords, jni_double_vector, 3 * numberArrows);

        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3 * i] = vx[2 * i + 1];
            arrowCoords[3 * i + 1] = vy[2 * i + 1];

            if (vz != NULL)
            {
                arrowCoords[3 * i + 2] = vz[2 * i + 1];
            }
            else
            {
                arrowCoords[3 * i + 2] = 0.0;
            }
        }

        setGraphicObjectProperty(pobjUID, __GO_DIRECTION__, arrowCoords, jni_double_vector, 3 * numberArrows);

        if (flag == 1)
        {
            /* Style is an array of numberArrows elements */
            setGraphicObjectProperty(pobjUID, __GO_SEGS_COLORS__, style, jni_int_vector, numberArrows);
        }
        else
        {
            /* Style is a scalar */
            setGraphicObjectProperty(pobjUID, __GO_SEGS_COLORS__, style, jni_int_vector, 1);
        }

    }
    else
    {
        /*
         * Type 1 corresponds to a CHAMP object
         * so building comes from champg
         */
        setGraphicObjectProperty(pobjUID, __GO_BASE_X__, vx, jni_double_vector, Nbr1);
        setGraphicObjectProperty(pobjUID, __GO_BASE_Y__, vy, jni_double_vector, Nbr2);

        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3 * i] = vfx[i];
            arrowCoords[3 * i + 1] = vfy[i];
            arrowCoords[3 * i + 2] = 0.0;
        }

        setGraphicObjectProperty(pobjUID, __GO_DIRECTION__, arrowCoords, jni_double_vector, 3 * numberArrows);

        /* typeofchamp corresponds to COLORED (0: false, 1: true) */
        setGraphicObjectProperty(pobjUID, __GO_COLORED__, &typeofchamp, jni_bool, 1);
    }

    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    /* Initializes the default Contour values */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    FREE(arrowCoords);

    return pobjUID;
}

/**sciConstructCompound
 * constructs a Compound of entities
 * do only an association with a parent and a handle reservation !
 * check for valid handle can be done using CheckForCompound
 */
char *ConstructCompound(long *handelsvalue, int number) /* Conflicting types with definition */
{
    char *compoundUID = NULL;
    char *parentAxesUID = NULL;
    char *firstMovedObjectUID = NULL;

    int i = 0;
    int parentVisible = 0;
    int *piParentVisible = &parentVisible;

    compoundUID = createGraphicObject(__GO_COMPOUND__);

    /* Add the Compound's handle */
    /* The Compound's parent Axes is considered to be the Compound's first child's own parent */
    firstMovedObjectUID = (char*)getObjectFromHandle((long)handelsvalue[0]);
    getGraphicObjectProperty(firstMovedObjectUID, __GO_PARENT__, jni_string, (void **)&parentAxesUID);

    /* Set the parent-child relationship between the Compound and each aggregated object */
    for (i = 0; i < number; i++)
    {
        char *movedObjectUID = (char*)getObjectFromHandle((long)handelsvalue[i]);

        setGraphicObjectRelationship(compoundUID, movedObjectUID);
    }

    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(parentAxesUID, compoundUID);

    getGraphicObjectProperty(parentAxesUID, __GO_VISIBLE__, jni_bool, (void **)&piParentVisible);
    setGraphicObjectProperty(compoundUID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    releaseGraphicObjectProperty(__GO_PARENT__, parentAxesUID, jni_string, 1);

    return (char *)compoundUID;
}

/**ConstructCompoundSeq
 * constructs a Compound of with the last n entities created in the current subwindow
 on entry the subwin children list is
 s1->s2->...->sn->sn+1->...->sN
 with sn the least recent of the last n entities created and s1 the most recent one
 (that is, the last entity created), and N the subwin's initial number of children
 on exit it is
 A->sn+1->sn+2->...->sN
 with A a Compound object whose children list is:
 s1->s2->...->sn-1->sn
*/
char *ConstructCompoundSeq(int number)
{
    char **children = NULL;
    char *parentFigure = NULL;
    int numberChildren = 0;
    int *piNumberChildren = &numberChildren;
    int i = 0;
    int visible = 0;
    int *piVisible = &visible;

    char *pobjUID = NULL;
    char const* psubwinUID = getCurrentSubWin();

    /* Creates the Compound object A */
    pobjUID = createGraphicObject(__GO_COMPOUND__);

    /* Add the Compound's handle */
    getGraphicObjectProperty(psubwinUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piNumberChildren);

    getGraphicObjectProperty(psubwinUID, __GO_CHILDREN__, jni_string_vector, (void **)&children);

    /*
     * Remove the last "number" created objects (located at the children list's head)
     * and add them to the compound in the same order
     */
    for (i = 0; i < number; i++)
    {
        /*
         * Set the parent-child relationship between the Compound and each aggregated object.
         * Children are added to the Compound from the least recent to the most recent, to
         * preserve their former ordering.
         */
        setGraphicObjectRelationship(pobjUID, children[number - i - 1]);
    }
    releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_string_vector, numberChildren);

    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(psubwinUID, pobjUID);

    /*
     * visibility is obtained from the parent Figure, whereas it is retrieved from the
     * parent Axes in ConstructCompound.
     * To be made consistent.
     */
    getGraphicObjectProperty(pobjUID, __GO_PARENT_FIGURE__, jni_string, (void **)&parentFigure);
    getGraphicObjectProperty(parentFigure, __GO_VISIBLE__, jni_bool, (void **)&piVisible);
    releaseGraphicObjectProperty(__GO_PARENT_FIGURE__, parentFigure, jni_string, 1);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    return (char *)pobjUID;
}

/**ConstructLabel
 * This function creates the Label structure used for x,y,z labels and for the Title.
 * On the contrary to the other Construct functions, it clones the Axes model's relevant
 * label instead of creating a new Label object and performing property sets using
 * the values of the Axes model's label, which is done instead by the clone call.
 * @param  char *pparentsubwinUID: the parent Axes' identifier.
 * @param  char text[] : initial text string, unused.
 * @param  int type to get info. on the type of label.
 */
void ConstructLabel(char * pparentsubwinUID, char const* text, int type)
{
    int const labelProperties[] = { __GO_X_AXIS_LABEL__, __GO_Y_AXIS_LABEL__, __GO_Z_AXIS_LABEL__, __GO_TITLE__ };
    int parentType = -1;
    int *piParentType = &parentType;
    int labelType = 0;
    char *modelLabelUID = NULL;
    char *pobjUID = NULL;
    int autoPosition = 0;
    int *piAutoPosition = &autoPosition;
    double position[3] = { 1.0, 1.0, 1.0 };

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_int, (void**)&piParentType);

    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return;
    }

    if (type < 1 || type > 4)
    {
        return;
    }

    labelType = labelProperties[type - 1];

    getGraphicObjectProperty(getAxesModel(), labelType, jni_string, (void **)&modelLabelUID);

    /* Creates a new Label object with the same properties as the Axes model's corresponding label */
    pobjUID = cloneGraphicObject(modelLabelUID);

    /* Position set to {1, 1, 1} as a default to take into account logarithmic coordinates */
    setGraphicObjectProperty(pobjUID, __GO_POSITION__, position, jni_double_vector, 3);

    /* Auto position must be reset as setting the position has set it to false */
    getGraphicObjectProperty(modelLabelUID, __GO_AUTO_POSITION__, jni_bool, (void **)&piAutoPosition);
    setGraphicObjectProperty(pobjUID, __GO_AUTO_POSITION__, &autoPosition, jni_bool, 1);

    /* Attach the cloned label to its parent Axes and set the latter as the label's parent */
    setGraphicObjectProperty(pparentsubwinUID, labelType, pobjUID, jni_string, 1);
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    releaseGraphicObjectProperty(labelType, modelLabelUID, jni_string, 1);
    releaseGraphicObjectProperty(__GO_PARENT__, pobjUID, jni_string, 1);
}
