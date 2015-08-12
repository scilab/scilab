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
 * Gets the specular color of the light/material.
 */
int get_specular_color_property(void* _pvCtx, int iObjUID)
{
    double* color = NULL;

    getGraphicObjectProperty(iObjUID, __GO_SPECULARCOLOR__, jni_double_vector, (void **)&color);

    if (color == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "specular_color");
        return -1;
    }

    return sciReturnMatrix(_pvCtx, color, 1, 3);

}
