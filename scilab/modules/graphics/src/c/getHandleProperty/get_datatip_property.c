/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
 * Gets the datatip data 3 component vector.
 */
int get_tip_data_property(void* _pvCtx, char* pobjUID)
{
    double *tip_data = NULL;

    getGraphicObjectProperty(pobjUID, __GO_DATATIP_DATA__, jni_double_vector, (void **)&tip_data);

    if (tip_data == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_data");
        return -1;
    }

    return sciReturnRowVector(_pvCtx, tip_data, 3);
}


/**
 * Get the datatip orientation.
 */
int get_tip_orientation_property(void* _pvCtx, char* pobjUID)
{
    int tip_orientation;
    int *piTipOrientation = &tip_orientation;

    getGraphicObjectProperty(pobjUID, __GO_DATATIP_ORIENTATION__, jni_int, (void **)&piTipOrientation);

    if (piTipOrientation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_orientation");
        return -1;
    }

    return sciReturnInt(_pvCtx, tip_orientation);
}

/**
 * Get the status if the Z component is displayed.
 */
int get_tip_3component_property(void* _pvCtx, char* pobjUID)
{
    int tip_3component;
    int *piTip_3component = &tip_3component;

    getGraphicObjectProperty(pobjUID, __GO_DATATIP_3COMPONENT__, jni_bool, (void **)&piTip_3component);

    if (piTip_3component == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_3component");
        return -1;
    }

    if (tip_3component)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
