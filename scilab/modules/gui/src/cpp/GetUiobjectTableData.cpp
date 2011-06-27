/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Get the table data of an uicontrol
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectTableData.hxx"

extern "C"
{
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
}

int GetUiobjectTableData(char *sciObjUID)
{
    int iNbTableData = 0;
    int *piNbTableData = &iNbTableData;
    char **pstTableData;

    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_TABLEDATA_SIZE__), jni_int, (void **) &piNbTableData);
    getGraphicObjectProperty(sciObjUID, const_cast<char*>(__GO_UI_TABLEDATA__), jni_string_vector, (void **) &pstTableData);

    if (pstTableData != NULL)
    {
        return sciReturnStringMatrix(pstTableData, 1, iNbTableData);
    }
    else
    {
        Scierror(999,const_cast<char*>(_("No '%s' property for this object.\n")), "TableData");
        return FALSE;
    }
}
