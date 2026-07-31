/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_font_color_alpha_property.c                                  */
/* desc : function to retrieve in Scilab the font_color_alpha field       */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_font_color_alpha_property(void* _pvCtx, int iObjUID)
{
    double dblFontColorAlpha = 1.0;
    double* pdblFontColorAlpha = &dblFontColorAlpha;

    getGraphicObjectProperty(iObjUID, __GO_FONT_COLOR_ALPHA__, jni_double, (void**) &pdblFontColorAlpha);

    if (pdblFontColorAlpha == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "font_color_alpha");
        return NULL;
    }

    return sciReturnDouble(dblFontColorAlpha);
}
/*------------------------------------------------------------------------*/
