/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: set_font_foreground_alpha_property.c                             */
/* desc : function to modify in Scilab the font_foreground_alpha field    */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_font_foreground_alpha_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double fontColorAlpha;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "font_foreground_alpha");
        return SET_PROPERTY_ERROR;
    }

    fontColorAlpha = ((double*)_pvData)[0];

    if (fontColorAlpha < 0.0 || fontColorAlpha > 1.0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be between 0 and 1.\n"), "font_foreground_alpha");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_FONT_COLOR_ALPHA__, &fontColorAlpha, jni_double, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }

    Scierror(999, _("'%s' property does not exist for this handle.\n"), "font_foreground_alpha");
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
