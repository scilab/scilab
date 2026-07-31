/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_mark_foreground_alpha_property.c                             */
/* desc : function to retrieve in Scilab the mark_foreground_alpha field  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_mark_foreground_alpha_property(void* _pvCtx, int iObjUID)
{
    double dblMarkForegroundAlpha = 1.0;
    double* pdblMarkForegroundAlpha = &dblMarkForegroundAlpha;

    getGraphicObjectProperty(iObjUID, __GO_MARK_FOREGROUND_ALPHA__, jni_double, (void**) &pdblMarkForegroundAlpha);

    if (pdblMarkForegroundAlpha == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_foreground_alpha");
        return NULL;
    }

    return sciReturnDouble(dblMarkForegroundAlpha);
}
/*------------------------------------------------------------------------*/
