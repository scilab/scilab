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
        int auto_orientation = 0;
        setGraphicObjectProperty(iObj, __GO_DATATIP_AUTOORIENTATION__, &auto_orientation, jni_bool, 1);
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "orientation");
        return SET_PROPERTY_ERROR;
    }
}

/**
 * Old z_componet property, warns the user
 */
int set_tip_z_component_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    char * tip_display_components;
    getGraphicObjectProperty(iObj, __GO_DATATIP_DISPLAY_COMPONENTS__, jni_string, (void **)&tip_display_components);

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
 * Set which coordinate components should be displayed
 */
int set_tip_display_components_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    const char * value = (const char*) _pvData;

    int isXSet = 0;
    int isYSet = 0;
    int isZSet = 0;
    //check if the value is valid and throws properly error msg
    int i;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "display_components");
        return SET_PROPERTY_ERROR;
    }

    for (i = 0; i < 4; ++i)
    {
        if (value[i] == '\0')
        {
            if (i == 0)
            {
                Scierror(999, _("Wrong value for '%s' property: Non-empty string expected.\n"), "display_components");
                return SET_PROPERTY_ERROR;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (i == 3)
            {
                Scierror(999, _("Wrong value for '%s' property: String with length 3 or less expected.\n"), "display_components");
                return SET_PROPERTY_ERROR;
            }
            switch (value[i])
            {
                case 'x':
                case 'X':
                    if (isXSet)
                    {
                        Scierror(999, _("Wrong value for '%s' property: String contains 'x' component more than once.\n"), "display_components");
                        return SET_PROPERTY_ERROR;
                    }
                    isXSet++;
                    break;
                case 'y':
                case 'Y':
                    if (isYSet)
                    {
                        Scierror(999, _("Wrong value for '%s' property: String contains 'y' component more than once.\n"), "display_components");
                        return SET_PROPERTY_ERROR;
                    }
                    isYSet++;
                    break;
                case 'z':
                case 'Z':
                    if (isZSet)
                    {
                        Scierror(999, _("Wrong value for '%s' property: String contains 'z' component more than once.\n"), "display_components");
                        return SET_PROPERTY_ERROR;
                    }
                    isZSet++;
                    break;
                default:
                    Scierror(999, _("Wrong value for '%s' property: String with 'x|y|z' expected.\n"), "display_components");
                    return SET_PROPERTY_ERROR;
            }
        }
    }

    status = setGraphicObjectProperty(iObj, __GO_DATATIP_DISPLAY_COMPONENTS__, value, jni_string, 1);
    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "display_components");
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

int set_tip_detached_property(void* _pvCtx, int iObj, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int isDetached = nbRow * nbCol != 0;
    double* detached_position = NULL;
    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Matrix expected.\n"), "detached_position");
        return SET_PROPERTY_ERROR;
    }


    if (nbRow * nbCol != 3 && isDetached)
    {
        Scierror(999, _("Wrong size for '%s' property: Matrix with length 3 or [] expected.\n"), "detached_position");
        return SET_PROPERTY_ERROR;
    }
    status = setGraphicObjectProperty(iObj, __GO_DATATIP_DETACHED_MODE__, &isDetached, jni_bool, 1);
    if (isDetached)
    {
        status = setGraphicObjectProperty(iObj, __GO_DATATIP_DETACHED_POSITION__, _pvData, jni_double_vector, 3);
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "detached_position");
        return SET_PROPERTY_ERROR;
    }
}
