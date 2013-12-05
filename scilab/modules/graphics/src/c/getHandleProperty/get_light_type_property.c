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
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


/**
 * Gets the type of the light.
 */
int get_light_type_property(void* _pvCtx, int iObjUID)
{
    int type = 0;
    int* piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_LIGHT_TYPE__, jni_int, (void **)&piType);

    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "light_type");
        return -1;
    }

    if (type == 0)
    {
        return sciReturnString(_pvCtx, "directional");
    }
    else
    {
        return sciReturnString(_pvCtx, "point");
    }

}
