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
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"


/**
 * Gets the shinines level of the surface.
 */
int get_material_shinines_property(void* _pvCtx, char* pobjUID)
{
    double shinines = 0;
    double* pShinines = &shinines;

    getGraphicObjectProperty(pobjUID, __GO_MATERIAL_SHININES__, jni_double, (void **)&pShinines);

    if (pShinines == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "material_shinines");
        return -1;
    }

    return sciReturnDouble(_pvCtx, shinines);

}
