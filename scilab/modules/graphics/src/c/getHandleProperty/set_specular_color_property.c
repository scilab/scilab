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

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"


#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Sets the specular color of the light/material.
 */
int set_specular_color_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double * color = NULL;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "specular_color");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 3)
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "specular_color", 3);
        return SET_PROPERTY_ERROR;
    }

    color = (double*)_pvData;
    if (color[0] < 0.0 || color[1] < 0.0 || color[2] < 0.0 ||
            color[0] > 1.0 || color[1] > 1.0 || color[2] > 1.0 )
    {
        Scierror(999, _("Wrong values for '%s' property: values between [0, 1] expected.\n"), "specular_color");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_SPECULARCOLOR__, _pvData, jni_double_vector, 3);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "specular_color");
        return SET_PROPERTY_ERROR;
    }
}
