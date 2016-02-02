/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
/* file: set_image_type_property.c                                         */
/* desc : function to modify in Scilab the image_type field of             */
/*        a matplot handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "os_string.h"

#include "Matplot.h"

/*------------------------------------------------------------------------*/
int set_image_type_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int imagetype = (int)MATPLOT_INDEX;
    int type = -1;
    int *piType = &type;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "image_type");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (type != __GO_MATPLOT__)
    {
        Scierror(999, _("Incompatible type for property %s.\n"), "image_type");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "rgb") == 0)
    {
        imagetype = (int)MATPLOT_RGB;
    }
    else if (stricmp((char*)_pvData, "rgba") == 0)
    {
        imagetype = (int)MATPLOT_RGBA;
    }
    else if (stricmp((char*)_pvData, "argb") == 0)
    {
        imagetype = (int)MATPLOT_ARGB;
    }
    else if (stricmp((char*)_pvData, "gray") == 0)
    {
        imagetype = (int)MATPLOT_GRAY;
    }
    else if (stricmp((char*)_pvData, "index") == 0)
    {
        imagetype = (int)MATPLOT_INDEX;
    }
    else if (stricmp((char*)_pvData, "red") == 0)
    {
        imagetype = (int)MATPLOT_RED;
    }
    else if (stricmp((char*)_pvData, "green") == 0)
    {
        imagetype = (int)MATPLOT_GREEN;
    }
    else if (stricmp((char*)_pvData, "blue") == 0)
    {
        imagetype = (int)MATPLOT_BLUE;
    }
    else if (stricmp((char*)_pvData, "rgb332") == 0)
    {
        imagetype = (int)MATPLOT_RGB_332;
    }
    else if (stricmp((char*)_pvData, "rgb444") == 0)
    {
        imagetype = (int)MATPLOT_RGB_444;
    }
    else if (stricmp((char*)_pvData, "rgba4444") == 0)
    {
        imagetype = (int)MATPLOT_RGBA_4444;
    }
    else if (stricmp((char*)_pvData, "rgb555") == 0)
    {
        imagetype = (int)MATPLOT_RGB_555;
    }
    else if (stricmp((char*)_pvData, "rgba5551") == 0)
    {
        imagetype = (int)MATPLOT_RGBA_5551;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "image_type", "rgb, rgba, gray, index");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, &imagetype, jni_int, 1);
    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("Invalid image type for this handle.\n"));
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
