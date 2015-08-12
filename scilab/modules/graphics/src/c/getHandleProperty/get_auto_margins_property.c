/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


/*------------------------------------------------------------------------*/
/* file: get_auto_margins_property.c                                        */
/* desc : function to retrieve in Scilab the auto_margins field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "localization.h"
#include "Scierror.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_auto_margins_property(void* _pvCtx, int iObjUID)
{
    int iAutoMargins = 0;
    int* piAutoMargins = &iAutoMargins;

    getGraphicObjectProperty(iObjUID, __GO_AUTO_MARGINS__, jni_bool, (void **)&piAutoMargins);

    if (piAutoMargins == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_margins");
        return -1;
    }

    if (iAutoMargins)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }

}
/*------------------------------------------------------------------------*/
