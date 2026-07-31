/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_color_alpha_property.c                                       */
/* desc : function to retrieve in Scilab the color_alpha field            */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_color_alpha_property(void* _pvCtx, int iObjUID)
{
    double dblColorAlpha = 1.0;
    double* pdblColorAlpha = &dblColorAlpha;

    getGraphicObjectProperty(iObjUID, __GO_LINE_COLOR_ALPHA__, jni_double, (void**) &pdblColorAlpha);

    if (pdblColorAlpha == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "color_alpha");
        return NULL;
    }

    return sciReturnDouble(dblColorAlpha);
}
/*------------------------------------------------------------------------*/
