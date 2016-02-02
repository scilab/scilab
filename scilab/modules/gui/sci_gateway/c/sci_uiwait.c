/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
#include <string.h>
#include "gw_gui.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "ContextMenu.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_uiwait(char *fname, void* pvApiCtx)
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
