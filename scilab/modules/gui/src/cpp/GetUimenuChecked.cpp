/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUimenuChecked.hxx"

int GetUimenuChecked(void* _pvCtx, char *pObjUID)
{
    int checked = 0;
    int *piChecked = &checked;

    getGraphicObjectProperty(pObjUID, __GO_UI_CHECKED__, jni_bool, (void **)&piChecked);

    if (piChecked == NULL)
    {
        Scierror(999, const_cast < char *>(_("'%s' property does not exist for this handle.\n")), "Checked");

        return FALSE;
    }

    if (checked == TRUE)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
