/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * Set the tooltip string property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "SetUicontrol.h"
}

int SetUicontrolTooltipString(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int objectStyle = -1;
    int* piObjectStyle = &objectStyle;
    int type = -1;
    int* piType = &type;

    // Check type
    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "TooltipString");
        return SET_PROPERTY_ERROR;
    }

    // Label must be a character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string or a vector of strings expected.\n")), "TooltipString");
        return SET_PROPERTY_ERROR;
    }

    // Check size according to uicontrol style
    getGraphicObjectProperty(iObjUID, __GO_STYLE__, jni_int, (void**) &piObjectStyle);
    // Value can be string or a string vector
    if (nbCol > 1 && nbRow > 1)
    {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string or a vector of strings expected.\n")), "TooltipString");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_TOOLTIPSTRING__, (char**)_pvData, jni_string_vector, nbRow * nbCol);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "TooltipString");
        return SET_PROPERTY_ERROR;
    }
}
