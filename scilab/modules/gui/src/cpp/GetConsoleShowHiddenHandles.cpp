/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "GetConsoleShowHiddenHandles.h"
#include "SetPropertyStatus.h"
#include "localization.h"
#include "Scierror.h"
#include "returnProperty.h"
#include "BOOL.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int GetConsoleShowHiddenHandles(void* _pvCtx, char *pObjUID)
{
    int iShowHiddenHandles = 0;
    int *piShowHiddenHandles = &iShowHiddenHandles;

    getGraphicObjectProperty(pObjUID, __GO_SHOWHIDDENHANDLES__, jni_bool, (void **)&piShowHiddenHandles);

    if (piShowHiddenHandles == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "ShowHiddenHandles");

        return FALSE;
    }

    if (iShowHiddenHandles == TRUE)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
