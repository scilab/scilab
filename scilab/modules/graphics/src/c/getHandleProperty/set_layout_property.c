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
/* file: set_figure_name_property.c                                       */
/* desc : function to modify in Scilab the figure_name field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"
#include "os_string.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
/*------------------------------------------------------------------------*/
int set_layout_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    enum LayoutType layout = LAYOUT_NONE;
    int iLayout = 0;
    int* piLayout = &iLayout;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "layout");
        return SET_PROPERTY_ERROR;
    }


    //check if we can set layout
    getGraphicObjectProperty(iObjUID, __GO_LAYOUT_SET__, jni_bool, (void **)&piLayout);
    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout");
        return SET_PROPERTY_ERROR;
    }

    if (iLayout == 0)
    {
        Scierror(999, _("'%s' has already been set.\n"), "layout");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "none") == 0)
    {
        layout = LAYOUT_NONE;
    }
    else if (stricmp((char*)_pvData, "gridbag") == 0)
    {
        layout = LAYOUT_GRIDBAG;
    }
    else if (stricmp((char*)_pvData, "grid") == 0)
    {
        layout = LAYOUT_GRID;
    }
    else if (stricmp((char*)_pvData, "border") == 0)
    {
        layout = LAYOUT_BORDER;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s, %s or %s expected.\n"), "layout", "'none'", "'grid'", "'gridbag'", "'border'");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_LAYOUT__, &layout, jni_int, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
