/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "localization.h"
#include "stack-c.h"
#include "Scierror.h"
#include "ContextMenu.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_uiwait(char *fname, unsigned long fname_len)
{
    int nbRow = 0, nbCol = 0, stkAdr = 0;

    char *result = NULL;

    long hdl = 0;

    char *pObjUID = NULL;
    int iObjType = -1;
    int *piObjType = &iObjType;

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    if (VarType(1) == sci_handles)
    {
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }
        hdl = (unsigned long)*hstk(stkAdr);
        pObjUID = (char*)getObjectFromHandle(hdl);

        getGraphicObjectProperty(pObjUID, __GO_TYPE__, jni_int, (void **)&piObjType);
        if (iObjType == __GO_UICONTEXTMENU__)
        {
            result = uiWaitContextMenu(pObjUID);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Uicontextmenu");
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A graphic handle expected.\n"), fname, 1);
        return FALSE;
    }

    /* Create return variable */
    nbRow = (int)strlen(result);
    nbCol = 1;
    CreateVar(Rhs + 1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
    strcpy(cstk(stkAdr), result);

    LhsVar(1) = Rhs + 1;

    PutLhsVar();

    return TRUE;
}

/*--------------------------------------------------------------------------*/
