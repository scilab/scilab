/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the row names of a uicontrol.
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectRowNames.hxx"

int SetUiobjectRownames(char *sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;

    // Label must be a character string
    if (valueType != sci_strings) {
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string vector expected.\n")), "RowNames");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol != 1 && nbRow != 1) {
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string vector expected.\n")), "RowNames");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_ROWNAMES__), getStringMatrixFromStack(stackPointer), jni_string_vector, nbRow * nbCol);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }

    Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "RowNames");
    return SET_PROPERTY_ERROR;
}
