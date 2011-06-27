/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the column names of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectColumnNames.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

int GetUiobjectColumnNames(char *sciObjUID)
{
    int iNbColumnNames = 0;
    int *piNbColumnNames = &iNbColumnNames;
    char **pstColumnNames;

    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_COLUMNNAMES_SIZE__), jni_int, (void **) &piNbColumnNames);
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_COLUMNNAMES__), jni_string_vector, (void **) &pstColumnNames);

    if (pstColumnNames != NULL)
    {
        return sciReturnStringMatrix(pstColumnNames, 1, iNbColumnNames);
    }
    else
    {
        Scierror(999,const_cast<char*>(_("No '%s' property for this object.\n")), "ColumnNames");
        return FALSE;
    }
}
