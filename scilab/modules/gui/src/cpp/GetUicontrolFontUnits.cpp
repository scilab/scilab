/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font units of an uicontrol
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
#include "GetUicontrol.h"
}

int GetUicontrolFontUnits(void* _pvCtx, int iObjUID)
{
    char* fontUnits = NULL;
    int status = 0;

    getGraphicObjectProperty(iObjUID, __GO_UI_FONTUNITS__, jni_string, (void**) &fontUnits);

    if (fontUnits == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontUnits");
        return FALSE;
    }
    else
    {
        status = sciReturnString(_pvCtx, fontUnits);
        delete[] fontUnits;
        return status;
    }

}
