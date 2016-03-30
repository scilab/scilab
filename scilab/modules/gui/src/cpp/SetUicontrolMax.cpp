/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2012 - DIGITEO - Vincent COUVERT
 * Sets the max property of an uicontrol object
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

extern "C"
{
#include "SetUicontrol.h"
}

int SetUicontrolMax(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    double maxValue = 0.0;
    BOOL status = FALSE;
    double value = 0;
    double* pdblValue = &value;
    double minValue = 0.0;
    double* pdblMinValue = &minValue;
    int objectStyle = -1;
    int *piObjectStyle = &objectStyle;
    int type = -1;
    int* piType = &type;

    // Check type
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real expected.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow != 1)
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }

    /* Store the value in Scilab */
    maxValue = ((double*)_pvData)[0];

    /*
     * For Checkboxes and Radiobuttons: display a warning if the value is neither equal to Min nor Max
     */
    getGraphicObjectProperty(iObjUID, __GO_STYLE__, jni_int, (void**) &piObjectStyle);
    if (objectStyle == __GO_UI_CHECKBOX__ || objectStyle == __GO_UI_RADIOBUTTON__)
    {
        getGraphicObjectProperty(iObjUID, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(iObjUID, __GO_UI_VALUE__, jni_double, (void**) &pdblValue);

        if ((value != minValue) && (value != maxValue))
        {
            sciprint(const_cast<char*>(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n")), IntToStyle(objectStyle), "Min", "Max");
        }

    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_MAX__, &maxValue, jni_double, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Max");
        return SET_PROPERTY_ERROR;
    }
}
