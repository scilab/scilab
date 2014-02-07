/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "BOOL.h"
#include "stricmp.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_scrollable_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL bScrollable = FALSE;
    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "scrollable");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "on") == 0)
    {
        bScrollable = TRUE;
    }
    else if (stricmp((char*)_pvData, "off") == 0)
    {
        bScrollable = FALSE;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s, %s or %s expected.\n"), "scrollable", "'on'", "'off'");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_UI_SCROLLABLE__, &bScrollable, jni_bool, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "scrollable");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
