/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2018 - Stéphane Mottelet
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

int SetUicontrolSnapToTicks(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "snaptoticks");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_UI_SNAPTOTICKS__, &b, jni_bool, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "snaptoticks");
        return SET_PROPERTY_ERROR;
    }
    return SET_PROPERTY_SUCCEED;
}
