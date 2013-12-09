/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int get_use_color_material_property(void* _pvCtx, int iObjUID)
{
    int iColorMaterial = 0;
    int *piColorMaterial = &iColorMaterial;

    getGraphicObjectProperty(iObjUID, __GO_COLOR_MATERIAL__, jni_bool, (void **)&piColorMaterial);

    if (piColorMaterial == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "use_color_material");
        return -1;
    }

    if (iColorMaterial)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
