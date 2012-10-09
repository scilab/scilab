/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int set_current_axes_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    char * curAxesUID   = NULL;
    char * parentFigureUID = NULL;
    int type = -1;
    int *piType = &type;

    if (pobjUID != NULL)
    {
        /* This property should not be called on a handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    if ( !( valueType == sci_handles ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Handle expected.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    curAxesUID = (char*)getObjectFromHandle( getHandleFromStack( stackPointer ) );

    if ( curAxesUID == NULL)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a valid handle.\n"), "current_entity");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(curAxesUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type != __GO_AXES__)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a handle on axes.\n"), "current_axes");
        return SET_PROPERTY_ERROR;
    }

    /* The current Axes' parent Figure's selected child property must be updated */
    getGraphicObjectProperty(curAxesUID, __GO_PARENT__, jni_string, (void **)&parentFigureUID);
    setGraphicObjectProperty(parentFigureUID, __GO_SELECTED_CHILD__, curAxesUID, jni_string, 1);

    setCurrentSubWin(curAxesUID);

    /* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
    /* we must also set the current figure to the subwin's parent */
    if (!isCurrentFigure(parentFigureUID))
    {
        setCurrentFigure(parentFigureUID);
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
