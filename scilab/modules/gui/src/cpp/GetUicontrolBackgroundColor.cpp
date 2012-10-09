/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the background color of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolBackgroundColor.hxx"

int GetUicontrolBackgroundColor(void* _pvCtx, char *sciObjUID)
{
    double *tmp = NULL;
    int status = 0;

    getGraphicObjectProperty(sciObjUID, __GO_UI_BACKGROUNDCOLOR__, jni_double_vector, (void **) &tmp);

    if (tmp == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "BackgroundColor");
        return FALSE;
    }
    else
    {
        status = sciReturnRowVector(_pvCtx, tmp, 3);
        delete[] tmp;
        return status;
    }
}

