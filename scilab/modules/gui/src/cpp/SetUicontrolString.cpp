/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Set the string property of an uicontrol
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

int SetUicontrolString(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int objectStyle = -1;
    int *piObjectStyle = &objectStyle;
    int type = -1;
    int *piType = &type;

    // Check type
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    //[]
    if (valueType == sci_matrix && nbRow == 0 && nbCol == 0)
    {
        if (setGraphicObjectProperty(iObjUID, __GO_UI_STRING__, (char**)NULL, jni_string_vector, 0) == FALSE)
        {
            Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
            return SET_PROPERTY_ERROR;
        }
        return SET_PROPERTY_SUCCEED;
    }

    // Label must be a character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: string expected.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    // Check size according to uicontrol style
    getGraphicObjectProperty(iObjUID, __GO_STYLE__, jni_int, (void**) &piObjectStyle);

    switch (objectStyle)
    {
        case __GO_UI_EDIT__ :
        {
            // Value can be string or a string vector
            if (nbCol > 1 && nbRow > 1)
            {
                Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: string or vector of strings expected.\n")), "String");
                return SET_PROPERTY_ERROR;
            }
            break;
        }
        case __GO_UI_TABLE__ :
        {
            break;
        }
        case __GO_UI_LISTBOX__ :
        case __GO_UI_POPUPMENU__ :
        {
            //combo and list color

            // ["white",    "#FFFFFF";
            //  "black",    "#000000";
            //  "red",      "#FF0000";
            //  "green",    "#00FF00";
            //  "blue",     "#0000FF"]

            //Matrix must be n*2
            if (nbCol == 2)
            {
                if (setGraphicObjectProperty(iObjUID, __GO_UI_STRING_COLNB__, &nbCol, jni_int, 1) == FALSE)
                {
                    Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
                    return SET_PROPERTY_ERROR;
                }

                if (setGraphicObjectProperty(iObjUID, __GO_UI_STRING__, (char**)_pvData, jni_string_vector, nbRow * nbCol) == FALSE)
                {
                    Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
                    return SET_PROPERTY_ERROR;
                }

                if (setGraphicObjectProperty(iObjUID, __GO_UI_VALUE__, (double**)NULL, jni_double_vector, 0) == FALSE)
                {
                    Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
                    return SET_PROPERTY_ERROR;
                }

                return SET_PROPERTY_SUCCEED;
            }
            break;
        }
        default:
        {
            // Value must be only one string
            if (nbCol * nbRow > 1)
            {
                Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: string expected.\n")), "String");
                return SET_PROPERTY_ERROR;
            }
            break;
        }
    }

    if (setGraphicObjectProperty(iObjUID, __GO_UI_STRING_COLNB__, &nbCol, jni_int, 1) == FALSE)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_UI_STRING__, (char**)_pvData, jni_string_vector, nbRow * nbCol) == FALSE)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
        return SET_PROPERTY_ERROR;
    }

    //reinit value for popupmenu and listbox after setString.
    if (objectStyle == __GO_UI_POPUPMENU__ || objectStyle == __GO_UI_LISTBOX__)
    {
        if (setGraphicObjectProperty(iObjUID, __GO_UI_VALUE__, (double**)NULL, jni_double_vector, 0) == FALSE)
        {
            Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "String");
            return SET_PROPERTY_ERROR;
        }
    }
    return SET_PROPERTY_SUCCEED;
}
