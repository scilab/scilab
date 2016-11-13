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
int sci_uicontextmenu(char *fname, void* pvApiCtx)
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
