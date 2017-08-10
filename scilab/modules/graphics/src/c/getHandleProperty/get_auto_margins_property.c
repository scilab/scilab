/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
void* get_auto_margins_property(void* _pvCtx, int iObjUID)
{
    int iAutoMargins = 0;
    int* piAutoMargins = &iAutoMargins;

    getGraphicObjectProperty(iObjUID, __GO_AUTO_MARGINS__, jni_bool, (void **)&piAutoMargins);

    if (piAutoMargins == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_margins");
        return NULL;
    }

    if (iAutoMargins)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }

}
/*------------------------------------------------------------------------*/
