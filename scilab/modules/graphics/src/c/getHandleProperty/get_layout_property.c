/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
/* file: get_figure_name_property.c                                       */
/* desc : function to retrieve in Scilab the figure_name field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
/*------------------------------------------------------------------------*/
void* get_layout_property(void* _pvCtx, int iObjUID)
{
    int iLayout = 0;
    int* piLayout = &iLayout;
    getGraphicObjectProperty(iObjUID, __GO_LAYOUT__, jni_int, (void **)&piLayout);

    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout");
        return NULL;
    }

    switch (iLayout)
    {
        default :
        case LAYOUT_NONE :
            return sciReturnString("none");
        case LAYOUT_GRIDBAG :
            return sciReturnString("gridbag");
        case LAYOUT_GRID :
            return sciReturnString("grid");
        case LAYOUT_BORDER :
            return sciReturnString("border");
    }
}
/*------------------------------------------------------------------------*/
