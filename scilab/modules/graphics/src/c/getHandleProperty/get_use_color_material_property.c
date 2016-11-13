/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
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


#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Returns if the diffuse color is used (on/off).
 */
void* get_use_color_material_property(void* _pvCtx, int iObjUID)
{
    int iColorMaterial = 0;
    int *piColorMaterial = &iColorMaterial;

    getGraphicObjectProperty(iObjUID, __GO_COLOR_MATERIAL__, jni_bool, (void **)&piColorMaterial);

    if (piColorMaterial == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "use_color_material");
        return NULL;
    }

    if (iColorMaterial)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}
