/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font units of an uicontrol
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
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

void* GetUicontrolFontUnits(void* _pvCtx, int iObjUID)
{
    char* fontUnits = NULL;
    void* status = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_FONTUNITS__, jni_string, (void**) &fontUnits);

    if (fontUnits == NULL)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "FontUnits");
        return NULL;
    }
    else
    {
        status = sciReturnString( fontUnits);
        delete[] fontUnits;
        return status;
    }

}
