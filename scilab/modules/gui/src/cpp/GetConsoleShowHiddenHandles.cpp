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
#include "GetUicontrol.h"
}

void* GetConsoleShowHiddenHandles(void* _pvCtx, int iObjUID)
{
    int iShowHiddenHandles = 0;
    int *piShowHiddenHandles = &iShowHiddenHandles;

    getGraphicObjectProperty(iObjUID, __GO_SHOWHIDDENHANDLES__, jni_bool, (void **)&piShowHiddenHandles);

    if (piShowHiddenHandles == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "ShowHiddenHandles");

        return NULL;
    }

    if (iShowHiddenHandles == TRUE)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

void* GetConsoleShowHiddenProperties(void* _pvCtx, int iObjUID)
{
    int iShowHiddenProperties = 0;
    int *piShowHiddenProperties = &iShowHiddenProperties;

    getGraphicObjectProperty(iObjUID, __GO_SHOWHIDDENPROPERTIES__, jni_bool, (void **)&piShowHiddenProperties);

    if (piShowHiddenProperties == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "ShowHiddenProperties");
        return NULL;
    }

    if (iShowHiddenProperties == TRUE)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

void* GetConsoleUseDeprecatedLF(void* _pvCtx, int iObjUID)
{
    int iUseDeprecatedLF = 0;
    int *piUseDeprecatedLF = &iUseDeprecatedLF;

    getGraphicObjectProperty(iObjUID, __GO_USEDEPRECATEDLF__, jni_bool, (void **)&piUseDeprecatedLF);

    if (piUseDeprecatedLF == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "UseDeprecatedSkin");
        return NULL;
    }

    if (iUseDeprecatedLF == TRUE)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}
