/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2012 - DIGITEO - Vincent COUVERT
 * Sets the min property of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolMin.hxx"

int SetUicontrolMin(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    double minValue = 0.0;
    BOOL status = FALSE;
    double value = 0.0;
    double* pdblValue = &value;
    double maxValue = 0.0;
    double* pdblMaxValue = &maxValue;
    char* objectStyle = NULL;
    char* type = NULL;

    // Check type
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_TYPE__), jni_string, (void**) &type);
    if (strcmp(type, __GO_UICONTROL__) != 0)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Min");
        return SET_PROPERTY_ERROR;
    }

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real expected.\n")), "Min");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow != 1)
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "Min");
        return SET_PROPERTY_ERROR;
    }

    /* Store the value in Scilab */
    minValue = getDoubleFromStack(stackPointer);

    /*
     * For Checkboxes and Radiobuttons: display a warning if the value is neither equal to Min nor Max
     */
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_STYLE__), jni_string, (void**) &objectStyle);
    if ((strcmp(objectStyle, __GO_UI_CHECKBOX__) == 0) || (strcmp(objectStyle, __GO_UI_RADIOBUTTON__)) == 0)
    {
        getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_VALUE__), jni_double, (void**) &pdblValue);
        getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_MAX__), jni_double, (void**) &pdblMaxValue);

        if ((value != minValue) && (value != maxValue))
        {
            sciprint(const_cast<char*>(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n")), objectStyle, "Min", "Max");
        }

    }
    free(objectStyle);

    status = setGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_MIN__), &minValue, jni_double, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Min");
        return SET_PROPERTY_ERROR;
    }
}
