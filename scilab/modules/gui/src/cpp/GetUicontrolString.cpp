/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the string of an uicontrol
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

int GetUicontrolString(void* _pvCtx, int iObjUID)
{
    int iNbColStrings = 0;
    int *piNbColStrings = &iNbColStrings;
    int iNbStrings = 0;
    int *piNbStrings = &iNbStrings;
    char **pstString = NULL;

    getGraphicObjectProperty(iObjUID, __GO_UI_STRING_SIZE__, jni_int, (void **) &piNbStrings);
    getGraphicObjectProperty(iObjUID, __GO_UI_STRING_COLNB__, jni_int, (void **) &piNbColStrings);
    getGraphicObjectProperty(iObjUID, __GO_UI_STRING__, jni_string_vector, (void **) &pstString);
    if (pstString != NULL)
    {
        if (iNbStrings == 0 || iNbColStrings == 0)
        {
            return sciReturnEmptyMatrix(_pvCtx);
        }
        else if (iNbColStrings == 1)
        {
            return sciReturnStringMatrix(_pvCtx, pstString, 1, iNbStrings);
        }
        else
        {
            return sciReturnStringMatrix(_pvCtx, pstString, iNbStrings / iNbColStrings, iNbColStrings);
        }
    }
    else
    {
        Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "String");
        return FALSE;
    }
}
