/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
#include "GetConsoleShowHiddenHandles.h"
#include "SetPropertyStatus.h"
#include "localization.h"
#include "Scierror.h"
#include "returnProperty.h"
#include "BOOL.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
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
