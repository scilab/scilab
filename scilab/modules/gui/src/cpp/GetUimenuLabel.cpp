/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUimenuLabel.hxx"

/* Get the label of an uimenu */
int GetUimenuLabel(void* _pvCtx, char *pobjUID)
{
    char *label = NULL;
    int status = 0;

    getGraphicObjectProperty(pobjUID, __GO_UI_LABEL__, jni_string, (void **)&label);

    if (label == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Label");
        return FALSE;
    }
    else
    {
        status = sciReturnString(_pvCtx, label);
        free(label);
        return status;
    }
}
