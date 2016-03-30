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
 * Gets the ambient color of the light/material.
 */
void* get_ambient_color_property(void* _pvCtx, int iObjUID)
{
    double* color = NULL;

    getGraphicObjectProperty(iObjUID, __GO_AMBIENTCOLOR__, jni_double_vector, (void **)&color);

    if (color == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ambient_color");
        return NULL;
    }

    return sciReturnMatrix(color, 1, 3);
}
