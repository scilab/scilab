/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
 * Get the datatip orientation.
 */
void* get_tip_orientation_property(void* _pvCtx, int iObjUID)
{
    int tip_orientation;
    int *piTipOrientation = &tip_orientation;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_ORIENTATION__, jni_int, (void **)&piTipOrientation);

    if (piTipOrientation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "orientation");
        return NULL;
    }

    return sciReturnInt(tip_orientation);
}

/**
 * Old z_componet property, warns the user
 */
void* get_tip_z_component_property(void* _pvCtx, int iObjUID)
{
    char * tip_display_components;
    getGraphicObjectProperty(iObjUID, __GO_DATATIP_DISPLAY_COMPONENTS__, jni_string, (void **)&tip_display_components);

    //Only warns if the property exists for the object.
    if (tip_display_components == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "z_component");
    }
    else
    {
        Scierror(999, _("'%s' property is obsolete and will be removed, use '%s' instead.\n"), "z_component", "display_components");
    }

    return NULL;
}


/**
 * Get the datatip components that should be displayed
 */
void* get_tip_display_components_property(void* _pvCtx, int iObjUID)
{
    char * tip_display_components;
    getGraphicObjectProperty(iObjUID, __GO_DATATIP_DISPLAY_COMPONENTS__, jni_string, (void **)&tip_display_components);

    if (tip_display_components == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "display_components");
        return NULL;
    }

    return sciReturnString(tip_display_components);
}

/**
 * Get the status if the auto-orientation is enabled.
 */
void* get_tip_auto_orientation_property(void* _pvCtx, int iObjUID)
{
    int tip_auto_orientation;
    int *piTip_auto_orientation = &tip_auto_orientation;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_AUTOORIENTATION__, jni_bool, (void **)&piTip_auto_orientation);

    if (piTip_auto_orientation == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_orientation");
        return NULL;
    }

    if (tip_auto_orientation)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

/**
 * Get the datatip interpolation mode (on/off).
 */
void* get_tip_interp_mode_property(void* _pvCtx, int iObjUID)
{
    int tip_interp_mode;
    int *piTip_interp_mode = &tip_interp_mode;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_INTERP_MODE__, jni_bool, (void **)&piTip_interp_mode);

    if (piTip_interp_mode == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_mode");
        return NULL;
    }

    if (tip_interp_mode)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

/**
 * Get the datatip box mode (true or false).
 */
void* get_tip_box_mode_property(void* _pvCtx, int iObjUID)
{
    int tip_box_mode;
    int *piTip_box_mode = &tip_box_mode;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_BOX_MODE__, jni_bool, (void **)&piTip_box_mode);

    if (piTip_box_mode == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "box_mode");
        return NULL;
    }

    if (tip_box_mode)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

/**
 * Get the datatip label mode (true or false).
 */
void* get_tip_label_mode_property(void* _pvCtx, int iObjUID)
{
    int tip_label_mode;
    int *piTip_label_mode = &tip_label_mode;

    getGraphicObjectProperty(iObjUID, __GO_DATATIP_LABEL_MODE__, jni_bool, (void **)&piTip_label_mode);

    if (piTip_label_mode == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "label_mode");
        return NULL;
    }

    if (tip_label_mode)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}


/**
 * Get the datatip display function.
 */
void* get_tip_disp_function_property(void* _pvCtx, int iObjUID)
{
    char *tip_disp_function = NULL;
    getGraphicObjectProperty(iObjUID, __GO_DATATIP_DISPLAY_FNC__, jni_string, (void **)&tip_disp_function);

    if (tip_disp_function == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "display_function");
        return NULL;
    }

    return sciReturnString(tip_disp_function);
}

void* get_tip_detached_property(void* _pvCtx, int iObjUID)
{
    int isDetached = 0;
    int *piDetached = &isDetached;
    getGraphicObjectProperty(iObjUID, __GO_DATATIP_DETACHED_MODE__, jni_bool, (void **)&piDetached);

    if (piDetached == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "detached_position");
        return NULL;
    }

    if (!isDetached)
    {
        return sciReturnEmptyMatrix();
    }
    else
    {
        double *detached_pos = NULL;
        getGraphicObjectProperty(iObjUID, __GO_DATATIP_DETACHED_POSITION__, jni_double_vector, (void **)&detached_pos);
        return sciReturnRowVector(detached_pos, 3);
    }
}
