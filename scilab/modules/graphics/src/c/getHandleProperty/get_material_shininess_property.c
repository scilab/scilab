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
 * Gets the shininess level of the surface.
 */
int get_material_shininess_property(void* _pvCtx, int iObjUID)
{
    double shininess = 0;
    double* pShininess = &shininess;

    getGraphicObjectProperty(iObjUID, __GO_MATERIAL_SHININESS__, jni_double, (void **)&pShininess);

    if (pShininess == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "material_shininess");
        return -1;
    }

    return sciReturnDouble(_pvCtx, shininess);

}
