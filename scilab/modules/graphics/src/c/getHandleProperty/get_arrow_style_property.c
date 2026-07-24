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
/* file: get_arrow_style_property.c                                        */
/* desc : function to retrieve in Scilab the arrow_style field of          */
/*        a Champ handle                                                    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_arrow_style_property(void* _pvCtx, int iObjUID)
{
    int arrowStyle = 0;
    int* piArrowStyle = &arrowStyle;

    getGraphicObjectProperty(iObjUID, __GO_ARROW_STYLE__, jni_int, (void **)&piArrowStyle);

    if (piArrowStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "arrow_style");
        return NULL;
    }

    if (arrowStyle == 1)
    {
        return sciReturnString("vback");
    }
    else
    {
        return sciReturnString("filled");
    }
}
/*------------------------------------------------------------------------*/
