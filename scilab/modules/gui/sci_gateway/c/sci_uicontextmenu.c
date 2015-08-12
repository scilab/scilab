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

#include <stdio.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_gui.h"
#include "gw_gui.h"
#include "localization.h"
#include "api_scilab.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
/*--------------------------------------------------------------------------*/
int sci_uicontextmenu(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    long long* stkAdr = NULL;
    int nbRow = 0, nbCol = 0;
    unsigned long GraphicHandle = 0;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* Create a new context menu */
    GraphicHandle = getHandle(createGraphicObject(__GO_UICONTEXTMENU__));

    /* Create return variable */
    nbRow = 1;
    nbCol = 1;

    sciErr = allocMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &stkAdr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    *stkAdr = GraphicHandle;

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
