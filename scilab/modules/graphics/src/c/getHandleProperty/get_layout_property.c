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
int get_layout_property(void* _pvCtx, int iObjUID)
{
    int iLayout = 0;
    int* piLayout = &iLayout;
    getGraphicObjectProperty(iObjUID, __GO_LAYOUT__, jni_int, (void **)&piLayout);

    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout");
        return -1;
    }

    switch (iLayout)
    {
        default :
        case LAYOUT_NONE :
            return sciReturnString(_pvCtx, "none");
        case LAYOUT_GRIDBAG :
            return sciReturnString(_pvCtx, "gridbag");
        case LAYOUT_GRID :
            return sciReturnString(_pvCtx, "grid");
        case LAYOUT_BORDER :
            return sciReturnString(_pvCtx, "border");
    }
}
/*------------------------------------------------------------------------*/
