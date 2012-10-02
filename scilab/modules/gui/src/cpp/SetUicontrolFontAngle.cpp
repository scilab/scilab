/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the font angle of an uicontrol object
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontAngle.hxx"

int SetUicontrolFontAngle(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    /* Font angle can be normal, italic or oblique */

    char* fontAngle = NULL;
    BOOL status = FALSE;

    // Font Name must be only one character string
    if (valueType != sci_strings) {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "FontAngle");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow == 0) {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "FontAngle");
        return SET_PROPERTY_ERROR;
    }

    fontAngle = getStringFromStack(stackPointer);

    if (stricmp(fontAngle, "normal") != 0
        && stricmp(fontAngle, "italic") != 0
        && stricmp(fontAngle, "oblique") != 0)
    {
        /* Wrong string format */
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n")), "FontAngle", "normal", "italic", "oblique");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObjUID, __GO_UI_FONTANGLE__, fontAngle, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontAngle");
        return SET_PROPERTY_ERROR;
    }
}
