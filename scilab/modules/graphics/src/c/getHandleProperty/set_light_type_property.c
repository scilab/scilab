/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
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

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


/**
 * Sets the type of the light.
 */
int set_light_type_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int val;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "light_type");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "directional") == 0)
    {
        val = 0;
    }
    else if (stricmp((char*)_pvData, "point") == 0)
    {
        val = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: 'directional' or 'point' expected.\n"), "light_type");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_LIGHT_TYPE__, &val, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "light_type");
        return SET_PROPERTY_ERROR;
    }
}
