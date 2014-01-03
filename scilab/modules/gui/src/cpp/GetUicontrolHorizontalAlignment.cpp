/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the horizontal alignment property of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetUicontrolHorizontalAlignment.hxx"

void* GetUicontrolHorizontalAlignment(void* _pvCtx, int iObjUID)
{
    char* alignment = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_HORIZONTALALIGNMENT__, jni_string, (void**) &alignment);

    if (alignment == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "HorizontalAlignment");
        return NULL;
    }
    else
    {
        status = sciReturnString(alignment);
        delete[] alignment;
        return status;
    }
}
