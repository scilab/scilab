/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Set the font name of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontName.hxx"

int SetUicontrolFontName(void* _pvCtx, char* sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    // Font Name must be only one character string

    BOOL status = FALSE;

    if (valueType != sci_strings) {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "FontName");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow == 0) {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "FontName");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObjUID, __GO_UI_FONTNAME__, getStringFromStack(stackPointer), jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontName");
        return SET_PROPERTY_ERROR;
    }
}
