/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_gui.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "ContextMenu.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_uiwait(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrstkAdr = NULL;
    long long* stkAdr = NULL;
    char* strAdr = NULL;

    int nbRow = 0, nbCol = 0;
    char *result = NULL;

    int iObjUID = 0;
    int iObjType = -1;
    int *piObjType = &iObjType;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((checkInputArgumentType(pvApiCtx, 1, sci_handles)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position 1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstkAdr, &nbRow, &nbCol, &stkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        iObjUID = getObjectFromHandle((unsigned long) * stkAdr);

        getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piObjType);
        if (iObjType == __GO_UICONTEXTMENU__)
        {
            result = uiWaitContextMenu(iObjUID);
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

    if (allocSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow * nbCol, (const char**) &strAdr))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    strcpy(strAdr, result);

    // TO DO : delete of "result"
    // uiWaitContextMenu(iObjUID) can return NULL.

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
