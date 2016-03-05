/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the font angle of an uicontrol object
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

int SetUicontrolFontAngle(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* Font angle can be normal, italic or oblique */

    char* fontAngle = NULL;
    BOOL status = FALSE;

    // Font Name must be only one character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: string expected.\n")), "FontAngle");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow == 0)
    {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: string expected.\n")), "FontAngle");
        return SET_PROPERTY_ERROR;
    }

    fontAngle = (char*)_pvData;

    if (stricmp(fontAngle, "normal") != 0
            && stricmp(fontAngle, "italic") != 0
            && stricmp(fontAngle, "oblique") != 0)
    {
        /* Wrong string format */
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n")), "FontAngle", "normal", "italic", "oblique");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_FONTANGLE__, fontAngle, jni_string, 1);

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
