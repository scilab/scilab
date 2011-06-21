/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_current_figure_property.c                                    */
/* desc : function to retrieve in Scilab the current_figure field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "CurrentFigure.h"
#include "CurrentObject.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "createGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "callJoGLView.h"

#include "CurrentFigure.h"
#include "FigureModel.h"
#include "AxesModel.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int get_current_figure_property(char *pobjUID)
{
    char *pstCurrentFigureId = NULL;

    if (pobjUID != NULL)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_figure");
        return -1;
    }

    /* return handle on the current figure */
    pstCurrentFigureId = getCurrentFigure();

    printf("[DEBUG] get_current_figure_property = %s\n", (pstCurrentFigureId == NULL ? "!! NULL !!" : pstCurrentFigureId));

    if (pstCurrentFigureId == NULL)
    {
        // No default figure : create a new one
        // Will be automatically set as the default one.
        char* pFigureUID = NULL;
        char* newaxesUID = NULL;
        int iID = 0;

        pFigureUID = cloneGraphicObject(getFigureModel());
        setGraphicObjectProperty(pFigureUID, __GO_ID__, &iID, jni_int, 1);
        createJoGLView(pFigureUID);
        /*
         * Clones a new Axes object using the Axes model which is then
         * attached to the newly created Figure.
         */
        newaxesUID = cloneGraphicObject(getAxesModel());

        /* Sets the parent-child relationship within the MVC */
        setGraphicObjectRelationship(pFigureUID, newaxesUID);

        /* Sets the newly created Axes as the Figure's current selected child */
        setGraphicObjectProperty(pFigureUID, __GO_SELECTED_CHILD__, newaxesUID, jni_string, 1);

        /*
         * Added back to avoid creating a new Figure each time gcf() is executed.
         * This was previously done in ConstructFigure, called by createFullFigure
         * which has been replaced by the Figure model clone call above.
         */
        // No more needed with MVC.
        //addNewFigureToList(pFigure);

        setCurrentFigure(pFigureUID);

        // Register handle to Scilab.
        //sciAddNewHandle(pFigure);

        /*
         * Registers the Axes' handle and sets the Axes as the current object.
         * This was previously done in ConstructSubWin, called by createFirstSubwin
         * which was also called by createFullFigure.
         */
        //sciAddNewHandle(newaxesUID);
        setCurrentObject(newaxesUID);

    }

    return sciReturnHandle(getHandle(getCurrentFigure()));

}
