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
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


/**
 * Gets the type of the light.
 */
void* get_light_type_property(void* _pvCtx, int iObjUID)
{
    int type = 0;
    int* piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_LIGHT_TYPE__, jni_int, (void **)&piType);

    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "light_type");
        return NULL;
    }

    if (type == 0)
    {
        return sciReturnString("directional");
    }
    else
    {
        return sciReturnString("point");
    }
}
