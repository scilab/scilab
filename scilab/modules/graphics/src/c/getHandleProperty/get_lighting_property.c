/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 * Get the lighting status (on/off).
 */
int get_lighting_property(void* _pvCtx, char* pobjUID)
{
    int iLighting = 0;
    int *piLighting = &iLighting;

    getGraphicObjectProperty(pobjUID, __GO_LIGHTING__, jni_bool, (void **)&piLighting);

    if (piLighting == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "lighting");
        return -1;
    }

    if (iLighting)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
