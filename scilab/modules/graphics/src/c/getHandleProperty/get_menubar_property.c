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

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "BarType.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_menubar_property(void* _pvCtx, int iObjUID)
{
    int iMenu = 0;
    int* piMenu = &iMenu;
    getGraphicObjectProperty(iObjUID, __GO_MENUBAR__, jni_int, (void **)&piMenu);

    if (piMenu == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar");
        return -1;
    }

    switch (iMenu)
    {
        default :
        case NONE :
            return sciReturnString(_pvCtx, "none");
        case FIGURE :
            return sciReturnString(_pvCtx, "figure");
    }
}
/*------------------------------------------------------------------------*/
int get_menubar_visible_property(void* _pvCtx, int iObjUID)
{
    int iVisible = 0;
    int* piVisible = &iVisible;
    getGraphicObjectProperty(iObjUID, __GO_MENUBAR_VISIBLE__, jni_bool, (void **)&piVisible);

    if (piVisible == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "menubar_visible");
        return FALSE;
    }

    if (iVisible == 0)
    {
        return sciReturnString(_pvCtx, "off");
    }
    else
    {
        return sciReturnString(_pvCtx, "on");
    }
}
/*------------------------------------------------------------------------*/
