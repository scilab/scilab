/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Set the string property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolString(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    char* objectStyle = NULL;
    char* type = NULL;

    // Check type
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_TYPE__), jni_string, (void**) &type);
    if (strcmp(type, __GO_UICONTROL__) != 0)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    // Label must be a character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    // Check size according to uicontrol style
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_STYLE__), jni_string, (void**) &objectStyle);
    if ((strcmp(objectStyle, __GO_UI_LISTBOX__) == 0) || (strcmp(objectStyle, __GO_UI_POPUPMENU__)) == 0)
    {
        // Value can be string or a string vector
        if (nbCol > 1 && nbRow > 1)
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string or a vector of strings expected.\n")), "String");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (strcmp(objectStyle, __GO_UI_TABLE__) != 0) // All other styles except 'Table'
    {
        // Value must be only one string
        if (nbCol * nbRow > 1)
        {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "String");
            return SET_PROPERTY_ERROR;
        }
    }

    status = setGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_STRING_COLNB__), &nbCol, jni_int, 1);
    if (status == FALSE)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_STRING__), getStringMatrixFromStack(stackPointer), jni_string_vector, nbRow * nbCol);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }
}
