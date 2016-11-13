/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the callback string of an uicontrol or uimenu
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

void* GetUiobjectEnable(void* _pvCtx, int iObjUID)
{
    int enable = 0;
    int* piEnable = &enable;

    getGraphicObjectProperty(iObjUID, __GO_UI_ENABLE__, jni_bool, (void**) &piEnable);

    if (piEnable == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Enable");
        return NULL;
    }

    if (enable == TRUE)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}

