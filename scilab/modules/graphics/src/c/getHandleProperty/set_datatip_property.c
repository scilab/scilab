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

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/**
 * Sets the datatip data.
 */
int set_tip_data_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double *tip_data;

    if (!( valueType == sci_matrix ))
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "tip_data");
        return SET_PROPERTY_ERROR ;
    }

    /* We must have 4 elements */
    if ( nbRow * nbCol != 3 )
    {
        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "tip_data", 3);
        return SET_PROPERTY_ERROR;
    }

    tip_data = (double*)_pvData;


    status = setGraphicObjectProperty(pobj, __GO_DATATIP_DATA__, tip_data, jni_double_vector, 3);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_data");
        return SET_PROPERTY_ERROR;
    }
}



/**
 * Sets the datatip oriantation.
 */
int set_tip_orientation_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int tip_orientation;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "tip_orientation");
        return SET_PROPERTY_ERROR;
    }

    tip_orientation = (int)((double*)_pvData)[0];

    status = setGraphicObjectProperty(pobj, __GO_DATATIP_ORIENTATION__, &tip_orientation, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_orientation");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Enable/disable the datatip Z component to be displayed.
 */
int set_tip_3component_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int use_z = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tip_3component");
    if (use_z == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobj, __GO_DATATIP_3COMPONENT__, &use_z, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_3component");
        return SET_PROPERTY_ERROR;
    }
}


/**
 * Enable/disable the datatip auto-orientation.
 */
int set_tip_auto_orientation_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int auto_orientation = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tip_auto_orientation");
    if (auto_orientation == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobj, __GO_DATATIP_AUTOORIENTATION__, &auto_orientation, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_auto_orientation");
        return SET_PROPERTY_ERROR;
    }
}


/**
 * Set the datatip interpolation mode (on/off).
 */
int set_tip_interp_mode_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int interp_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tip_interp_mode");
    if (interp_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobj, __GO_DATATIP_INTERP_MODE__, &interp_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_interp_mode");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Set the datatip box mode (true or false).
 */
int set_tip_box_mode_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int box_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tip_box_mode");
    if (box_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobj, __GO_DATATIP_BOX_MODE__, &box_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_box_mode");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Set the datatip label mode (true or false).
 */
int set_tip_label_mode_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int label_mode = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tip_label_mode");
    if (label_mode == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(pobj, __GO_DATATIP_LABEL_MODE__, &label_mode, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_label_mode");
        return SET_PROPERTY_ERROR;
    }
}


int set_tip_disp_function_property(void* _pvCtx, char* pobj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "tip_disp_function");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(pobj, __GO_DATATIP_DISPLAY_FNC__, _pvData, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tip_disp_function");
        return SET_PROPERTY_ERROR;
    }
}