/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the vertical alignment of an uicontrol object
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

int SetUicontrolVerticalAlignment(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /* VerticalAlignment can be top, middle or bottom */

    char* alignment = NULL;
    BOOL status = FALSE;

    // Vertical Alignment must be only one character string
    if (valueType != sci_strings)
    {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: '%s', '%s', or '%s' expected.\n")), "VerticalAlignment", "top", "middle", "bottom");
        return SET_PROPERTY_ERROR;
    }
    if (nbCol != 1 || nbRow == 0)
    {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: '%s', '%s', or '%s' expected.\n")), "VerticalAlignment", "top", "middle", "bottom");
        return SET_PROPERTY_ERROR;
    }

    alignment = (char*)_pvData;

    if (stricmp(alignment, "top") != 0
            && strcmp(alignment, "middle") != 0
            && strcmp(alignment, "bottom") != 0)
    {
        /* Wrong string format */
        Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', or '%s' expected.\n")), "VerticalAlignment", "top", "middle", "bottom");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_VERTICALALIGNMENT__, alignment, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "VerticalAlignment");
        return SET_PROPERTY_ERROR;
    }
}
