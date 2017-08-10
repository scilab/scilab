/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
#include "SetConsoleShowHiddenHandles.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "localization.h"
#include "Scierror.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int SetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, const_cast < char *>("ShowHiddenHandles"));

    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_SHOWHIDDENHANDLES__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ShowHiddenHandles");
        return SET_PROPERTY_ERROR;
    }
}

int SetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, const_cast < char *>("ShowHiddenProperties"));

    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_SHOWHIDDENPROPERTIES__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "ShowHiddenProperties");
        return SET_PROPERTY_ERROR;
    }
}

int SetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = (int)FALSE;
    BOOL status = FALSE;

    b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, const_cast < char *>("UseDeprecatedSkin"));

    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_USEDEPRECATEDLF__, &b, jni_bool, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "UseDeprecatedSkin");
        return SET_PROPERTY_ERROR;
    }
}

