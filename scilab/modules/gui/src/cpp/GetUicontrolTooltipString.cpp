/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * Get the tooltip string of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolTooltipString.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolTooltipString(void* _pvCtx, char *sciObjUID)
{
    int iNbStrings = 0;
    int *piNbStrings = &iNbStrings;
    char **pstString = NULL;

    getGraphicObjectProperty(sciObjUID, __GO_UI_TOOLTIPSTRING_SIZE__, jni_int, (void **) &piNbStrings);
    getGraphicObjectProperty(sciObjUID, __GO_UI_TOOLTIPSTRING__, jni_string_vector, (void **) &pstString);

    if (pstString != NULL)
    {
        return sciReturnStringMatrix(_pvCtx, pstString, 1, iNbStrings);
    }
    else
    {
        Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "TooltipString");
        return FALSE;
    }
}
