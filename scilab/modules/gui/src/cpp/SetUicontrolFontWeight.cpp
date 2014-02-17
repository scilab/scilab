/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the font weight of an uicontrol object
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

int SetUicontrolFontWeight(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* Font weight can be light, normal, demi or bold */

    char* fontWeight = NULL;
    BOOL status = FALSE;

    // Font Name must be only one character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "FontWeight");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow == 0)
    {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "FontWeight");
        return SET_PROPERTY_ERROR;
    }

    fontWeight = (char*)_pvData;

    if (stricmp(fontWeight, "light") != 0
            && stricmp(fontWeight, "normal") != 0
            && stricmp(fontWeight, "demi") != 0
            && stricmp(fontWeight, "bold") != 0)
    {
        /* Wrong string format */
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s' or '%s' expected.\n")), "FontWeight", "light", "normal", "demi", "bold");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_FONTWEIGHT__, fontWeight, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontWeight");
        return SET_PROPERTY_ERROR;
    }
}
