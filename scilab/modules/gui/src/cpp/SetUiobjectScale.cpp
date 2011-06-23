/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Sets the scale property of uicontrol imagerender
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectScale.hxx"

int SetUiobjectScale(char *sciObjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;

    if (valueType != sci_matrix)
    {
        /* Wrong datatype */
        Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A 1 x %d real row vector expected.\n")), "Scale", 2);
        return SET_PROPERTY_ERROR;
    }

    if((nbRow != 1) || (nbCol != 2))
    {
        /* Wrong value size */
        Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A 1 x %d real row vector expected.\n")), "Scale", 2);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_SCALE__), getDoubleMatrixFromStack(stackPointer), jni_double_vector, 2);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }

    Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Scale");
    return SET_PROPERTY_ERROR;
}
