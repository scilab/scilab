/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include <string.h>

#include "GetUicontrol.h"
}

/*------------------------------------------------------------------------*/
int GetUicontrolIcon(void* _pvCtx, int iObjUID)
{
    char* psticon = NULL;
    getGraphicObjectProperty(iObjUID, __GO_UI_ICON__, jni_string, (void **)&psticon);
    if (psticon == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "icon");
        return -1;
    }

    return sciReturnString(_pvCtx, psticon);
}
/*------------------------------------------------------------------------*/
