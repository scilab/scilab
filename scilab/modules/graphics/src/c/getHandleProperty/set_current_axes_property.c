/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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

/*------------------------------------------------------------------------*/
/* file: set_current_axes_property.c                                      */
/* desc : function to modify in Scilab the current_axes field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"
#include "sci_types.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"

/*------------------------------------------------------------------------*/
int set_current_axes_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iCurAxesUID = 0;
    int iCurChildUID = 0;
    int iParentFigureUID = -1;
    int* piParentFigureUID = &iParentFigureUID;
    int type = -1;
    int *piType = &type;

    if (iObjUID != 0)
    {
        /* This property should not be called on a handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    if (valueType != sci_handles)
    {
        Scierror(999, _("Wrong type for '%s' property: Handle expected.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    iCurAxesUID = getObjectFromHandle((long)((long long*)_pvData)[0]);

    if (iCurAxesUID == 0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a valid handle.\n"), "current_entity");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iCurAxesUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type != __GO_AXES__)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a handle on axes.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    setCurrentSubWin(iCurAxesUID);


    // Look for top level figure
    type = -1;
    iCurChildUID = iCurAxesUID;
    do
    {
        iParentFigureUID = getParentObject(iCurChildUID);
        getGraphicObjectProperty(iParentFigureUID, __GO_TYPE__, jni_int, (void **)&piType);
        iCurChildUID = iParentFigureUID;
    }
    while (iParentFigureUID != -1 && type != __GO_FIGURE__);

    /* The current Axes' parent Figure's selected child property must be updated */
    setGraphicObjectProperty(iParentFigureUID, __GO_SELECTED_CHILD__, &iCurAxesUID, jni_int, 1);

    /* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
    /* we must also set the current figure to the subwin's parent */
    if (!isCurrentFigure(iParentFigureUID))
    {
        setCurrentFigure(iParentFigureUID);
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
