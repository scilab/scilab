/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the callback string of an uicontrol or uimenu
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectEnable.hxx"

int GetUiobjectEnable(void* _pvCtx, char *pObjUID)
{
    int enable = 0;
    int* piEnable = &enable;

    getGraphicObjectProperty(pObjUID, __GO_UI_ENABLE__, jni_bool, (void**) &piEnable);

    if (piEnable == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Enable");
        return FALSE;
    }

    if (enable == TRUE)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}

