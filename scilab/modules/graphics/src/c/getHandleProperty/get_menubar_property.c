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

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "BarType.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_menubar_property(void* _pvCtx, int iObjUID)
{
    int iMenu = 0;
    int* piMenu = &iMenu;
    getGraphicObjectProperty(iObjUID, __GO_MENUBAR__, jni_int, (void **)&piMenu);

    if (piMenu == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar");
        return NULL;
    }

    switch (iMenu)
    {
        default :
        case NONE :
            return sciReturnString("none");
        case FIGURE :
            return sciReturnString("figure");
    }
}
/*------------------------------------------------------------------------*/
void* get_menubar_visible_property(void* _pvCtx, int iObjUID)
{
    int iVisible = 0;
    int* piVisible = &iVisible;
    getGraphicObjectProperty(iObjUID, __GO_MENUBAR_VISIBLE__, jni_bool, (void **)&piVisible);

    if (piVisible == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar_visible");
        return NULL;
    }

    if (iVisible == 0)
    {
        return sciReturnString("off");
    }
    else
    {
        return sciReturnString("on");
    }
}
/*------------------------------------------------------------------------*/
