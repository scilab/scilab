/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the units of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolUnits.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolUnits(void* _pvCtx, char *sciObjUID)
{
    char* units = NULL;
    int type = -1;
    int *piType = &type;

    /* Handle must be a uicontrol */
    getGraphicObjectProperty(sciObjUID, __GO_TYPE__, jni_int, (void**) &piType);
    if (type != __GO_UICONTROL__)
    {
        Scierror(999, const_cast<char*>(_("'%s' property does not exist for this handle.\n")), "Units");
        return FALSE;
    }

    getGraphicObjectProperty(sciObjUID, __GO_UI_UNITS__, jni_string, (void**) &units);
    return sciReturnString(_pvCtx, units);
}
