/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011-2012 - DIGITEO - Vincent COUVERT
 * Sets the value property of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>

#include "GetUicontrolStyle.hxx"
#include "SetUicontrolValue.hxx"
#include "stack-c.h"

int SetUicontrolValue(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    double *value = NULL;
    double* truncatedValue = NULL;
    int valueSize = 0;
    int nbValues = 0;
    int kValue = 0;
    BOOL status = FALSE;
    BOOL truncated = FALSE;
    double maxValue = 0;
    double* pdblMaxValue = &maxValue;
    double minValue = 0;
    double* pdblMinValue = &minValue;
    int objectStyle = -1;
    int *piObjectStyle = &objectStyle;
    int type = -1;
    int *piType = &type;

    // Check type
    getGraphicObjectProperty(sciObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
        return SET_PROPERTY_ERROR;
    }

    if (valueType == sci_matrix)
    {
        if (nbRow > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real row vector expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }

        value = stk(stackPointer);
        valueSize = nbCol * nbRow;
    }
    else if (valueType == sci_strings) // Ascendant compatibility
    {
        if (nbCol > 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }

        value = new double[1];
        valueSize = 1;
        nbValues = sscanf(getStringFromStack(stackPointer), "%lf", &value[0]);

        if (nbValues != 1)
        {
            /* Wrong value size */
            Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: A String containing a numeric value expected.\n")), "Value");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real row vector or a string expected.\n")), "Value");
        return SET_PROPERTY_ERROR;
    }

    truncatedValue = new double[valueSize];
    for (kValue = 0; kValue < valueSize; kValue++)
    {
        truncatedValue[kValue] = floor(value[kValue]);
        if (truncatedValue[kValue] != value[kValue])
        {
            truncated = TRUE;
        }
    }

    getGraphicObjectProperty(sciObjUID, __GO_STYLE__, jni_int, (void**) &piObjectStyle);

    /*
     * For popumenus/listboxes: display a warning if the value is not an integer
     */
    if ((objectStyle == __GO_UI_POPUPMENU__ || objectStyle == __GO_UI_LISTBOX__) && truncated)
    {
        sciprint(const_cast<char*>(_("Warning: '%s' 'Value' property should be an integer, the value will be truncated.\n")), IntToStyle(objectStyle));
    }

    /*
     * For Checkboxes and Radiobuttons: display a warning if the value is neither equal to Min nor Max
     */
    if (objectStyle == __GO_UI_CHECKBOX__ || objectStyle == __GO_UI_RADIOBUTTON__)
    {
        getGraphicObjectProperty(sciObjUID, __GO_UI_MIN__, jni_double, (void**) &pdblMinValue);
        getGraphicObjectProperty(sciObjUID, __GO_UI_MAX__, jni_double, (void**) &pdblMaxValue);

        if ((value[0] != minValue) && (value[0] != maxValue))
        {
            sciprint(const_cast<char*>(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n")), IntToStyle(objectStyle), "Min", "Max");
        }

    }

    if (objectStyle == __GO_UI_POPUPMENU__ || objectStyle == __GO_UI_LISTBOX__)
    {
        status = setGraphicObjectProperty(sciObjUID, __GO_UI_VALUE__, truncatedValue, jni_double_vector, valueSize);
    }
    else
    {
        status = setGraphicObjectProperty(sciObjUID, __GO_UI_VALUE__, value, jni_double_vector, valueSize);
    }

    delete[] truncatedValue;

    if (valueType == sci_strings)
    {
        delete[] value;
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Value");
        return SET_PROPERTY_ERROR;
    }
}
