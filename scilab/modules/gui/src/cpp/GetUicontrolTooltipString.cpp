/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * Get the tooltip string of an uicontrol
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

void* GetUicontrolTooltipString(void* _pvCtx, int iObjUID)
{
    int iNbStrings = 0;
    int *piNbStrings = &iNbStrings;
    char **pstString = NULL;
    void* ret = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_TOOLTIPSTRING_SIZE__, jni_int, (void **) &piNbStrings);
    getGraphicObjectProperty(iObjUID, __GO_UI_TOOLTIPSTRING__, jni_string_vector, (void **) &pstString);

    if (pstString != NULL)
    {
        ret = sciReturnStringMatrix(pstString, 1, iNbStrings);
        releaseGraphicObjectProperty(__GO_UI_TOOLTIPSTRING__, pstString, jni_string_vector, iNbStrings);
        return ret;
    }
    else
    {
        Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "TooltipString");
        return NULL;
    }
}
