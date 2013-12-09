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

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"


#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Sets the diffuse color of the light/material.
 */
int set_diffuse_color_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double * color;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "diffuse_color");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 3)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "diffuse_color", 3);
        return SET_PROPERTY_ERROR;
    }

    color = (double*)_pvData;
    if (color[0] < 0.0 || color[1] < 0.0 || color[2] < 0.0 ||
            color[0] > 1.0 || color[1] > 1.0 || color[2] > 1.0 )
    {
        Scierror(999, _("Wrong values for '%s' property: values between [0, 1] expected.\n"), "diffuse_color");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_DIFFUSECOLOR__, _pvData, jni_double_vector, 3);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "diffuse_color");
        return SET_PROPERTY_ERROR;
    }
}
