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

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_infobar_visible_property(void* _pvCtx, int iObjUID)
{
    int visible = 0;
    int* piVisible = &visible;

    getGraphicObjectProperty(iObjUID, __GO_INFOBAR_VISIBLE__, jni_bool, (void **)&piVisible);

    if (piVisible == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "infobar");
        return FALSE;
    }

    if (visible)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
/*------------------------------------------------------------------------*/
