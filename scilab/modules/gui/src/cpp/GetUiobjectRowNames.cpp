/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the row names of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectRowNames.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

int GetUiobjectRowNames(char *sciObjUID)
{
    int iNbRowNames = 0;
    int *piNbRowNames = &iNbRowNames;
    char **pstRowNames;

    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_ROWNAMES_SIZE__), jni_int, (void **) &piNbRowNames);
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_ROWNAMES__), jni_string_vector, (void **) &pstRowNames);

    if (pstRowNames != NULL)
    {
        return sciReturnStringMatrix(pstRowNames, 1, iNbRowNames);
    }
    else
    {
        Scierror(999,const_cast<char*>(_("No '%s' property for this object.\n")), "RowNames");
        return FALSE;
    }
}
