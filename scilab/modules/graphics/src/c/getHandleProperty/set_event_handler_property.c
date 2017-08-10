/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: set_event_handler_property.c                                     */
/* desc : function to modify in Scilab the event_handler field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_event_handler_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int bEnable = FALSE;
    char *pstHandler = NULL;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "event_handler");
        return SET_PROPERTY_ERROR;
    }

    pstHandler = (char*)_pvData;
    status = setGraphicObjectProperty(iObjUID, __GO_EVENTHANDLER_NAME__, pstHandler, jni_string, 1);

    if (strlen(pstHandler) == 0)
    {
        // f.event_handler = "" => f.event_handler_enable = "off"
        status = setGraphicObjectProperty(iObjUID, __GO_EVENTHANDLER_ENABLE__, &bEnable, jni_bool, 1);
    }


    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "event_handler");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
