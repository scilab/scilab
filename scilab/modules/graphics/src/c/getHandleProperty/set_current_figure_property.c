/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

/*------------------------------------------------------------------------*/
/* file: set_current_figure_property.c                                    */
/* desc : function to modify in Scilab the current_figure field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"
#include "sci_types.h"

#include "BuildObjects.h"
#include "CurrentFigure.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FigureList.h"
#include "CurrentSubwin.h"
#include "createGraphicObject.h"


/*------------------------------------------------------------------------*/
int set_current_figure_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int figNum = -1;
    int res = -1;
    int iFigureUID = 0;
    int iCurFigUID = 0;
    int iAxesUID = 0;
    int* piAxesUID = &iAxesUID;
    int iType = -1;
    int *piType = &iType;

    if (iObjUID != 0)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_figure");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: A scalar expected.\n"), "current_figure");
        return SET_PROPERTY_ERROR;
    }

    if ((valueType == sci_handles))
    {

        iCurFigUID = getObjectFromHandle((long)((long long*)_pvData)[0]);

        if (iCurFigUID == 0)
        {
            Scierror(999, _("'%s' handle does not or no longer exists.\n"), "Figure");
            return SET_PROPERTY_ERROR;
        }

        // Check new current figure is a figure
        getGraphicObjectProperty(iCurFigUID, __GO_TYPE__, jni_int,  (void**)&piType);
        if (iType != __GO_FIGURE__)
        {
            Scierror(999, _("Wrong value for '%s' property: A '%s' handle expected.\n"), "current_figure", "Figure");
            return SET_PROPERTY_ERROR;
        }

        setCurrentFigure(iCurFigUID);
        getGraphicObjectProperty(iCurFigUID, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxesUID);
        setCurrentSubWin(iAxesUID);

        return 0;
    }
    else if ((valueType == sci_matrix))
    {
        figNum = (int) ((double*)_pvData)[0];
    }
    else
    {
        Scierror(999, _("Wrong type for '%s' property: Real or '%s' handle expected.\n"), "current_figure", "Figure");
        return SET_PROPERTY_ERROR;
    }

    /* Retrieve figure with figNum */
    iFigureUID = getFigureFromIndex(figNum);

    if (iFigureUID == 0)
    {
        // No Figure available with this index, should create it  !!
        iFigureUID = createNewFigureWithAxes();
        setGraphicObjectProperty(iFigureUID, __GO_ID__, &figNum, jni_int, 1);
    }
    setCurrentFigure(iFigureUID);

    getGraphicObjectProperty(iFigureUID, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxesUID);
    setCurrentSubWin(iAxesUID);
    return 0;
}
/*------------------------------------------------------------------------*/
