/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2018 - Stéphane MOTTELET
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

void* GetUicontrolSnapToTicks(void* _pvCtx, int iObjUID)
{
    int iSnaptoticks = 0;
    int* piSnaptoticks = &iSnaptoticks;

    getGraphicObjectProperty(iObjUID, __GO_UI_SNAPTOTICKS__, jni_bool, (void **)&piSnaptoticks);

    if (piSnaptoticks == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "snaptoticks");
        return NULL;
    }

    if (iSnaptoticks == TRUE)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}
