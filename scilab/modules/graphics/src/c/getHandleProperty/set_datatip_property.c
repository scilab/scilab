/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Sets the datatip oriantation.
 */
int set_tip_orientation_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int tip_orientation;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "orientation");
        return SET_PROPERTY_ERROR;
    }

    tip_orientation = (int)((double*)_pvData)[0];

    status = setGraphicObjectProperty(iObj, __GO_DATATIP_ORIENTATION__, &tip_orientation, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "orientation");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Enable/disable the datatip Z component to be displayed.
 */
int set_tip_3component_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int use_z = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "z_component");
    if (use_z == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_3COMPONENT__, &use_z, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "z_component");
        return SET_PROPERTY_ERROR;
    }
}


/**
 * Enable/disable the datatip auto-orientation.
 */
int set_tip_auto_orientation_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int auto_orientation = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "auto_orientation");
    if (auto_orientation == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_AUTOORIENTATION__, &auto_orientation, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_orientation");
        return SET_PROPERTY_ERROR;
    }
}


/**
 * Set the datatip interpolation mode (on/off).
 */
int set_tip_interp_mode_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int interp_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "interp_mode");
    if (interp_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_INTERP_MODE__, &interp_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_mode");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Set the datatip box mode (true or false).
 */
int set_tip_box_mode_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int box_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "box_mode");
    if (box_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_BOX_MODE__, &box_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "box_mode");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Set the datatip label mode (true or false).
 */
int set_tip_label_mode_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int label_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "label_mode");
    if (label_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_LABEL_MODE__, &label_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "label_mode");
        return SET_PROPERTY_ERROR;
    }
}


int set_tip_disp_function_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "display_function");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObj, __GO_DATATIP_DISPLAY_FNC__, _pvData, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "display_function");
        return SET_PROPERTY_ERROR;
    }
}