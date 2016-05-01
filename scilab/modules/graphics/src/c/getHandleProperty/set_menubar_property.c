/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "BOOL.h"
#include "os_string.h"
#include "BarType.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_menubar_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    enum BarType menu = NONE;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "menubar");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "none") == 0)
    {
        menu = NONE;
    }
    else if (stricmp((char*)_pvData, "figure") == 0)
    {
        menu = FIGURE;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "menubar", "'none'", "'figure'");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_MENUBAR__, &menu, jni_int, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
int set_menubar_visible_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b =  tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "menubar_visible");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_MENUBAR_VISIBLE__, &b, jni_bool, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar_visible");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
