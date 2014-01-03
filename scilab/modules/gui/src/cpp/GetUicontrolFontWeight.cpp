/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the font weight of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "GetUicontrolFontWeight.hxx"

void* GetUicontrolFontWeight(void* _pvCtx, int iObjUID)
{
    char* fontWeight = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_FONTWEIGHT__, jni_string, (void**) &fontWeight);

    if (fontWeight == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontWeight");
        return NULL;
    }
    else
    {
        status = sciReturnString(fontWeight);
        delete[] fontWeight;
        return status;
    }
}
