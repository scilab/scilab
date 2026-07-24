/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: set_arrow_style_property.c                                        */
/* desc : function to modify in Scilab the arrow_style field of            */
/*        a Champ handle                                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_arrow_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int arrowStyle = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "arrow_style");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "filled") == 0)
    {
        arrowStyle = 0;
    }
    else if (stricmp((char*)_pvData, "vback") == 0)
    {
        arrowStyle = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "arrow_style", "filled", "vback");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_ARROW_STYLE__, &arrowStyle, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "arrow_style");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
