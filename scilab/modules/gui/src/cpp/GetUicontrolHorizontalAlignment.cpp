/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the horizontal alignment property of an uicontrol
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
