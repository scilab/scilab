/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: sci_xname.c                                                      */
/* desc : interface for xname routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

#include "BuildObjects.h"
#include "CurrentFigure.h"

#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xname(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    char* l1 = NULL;

    int iCurrentFigure = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    if (getAllocatedSingleString(pvApiCtx, piAddrl1, &l1))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    iCurrentFigure = getCurrentFigure();

    if (iCurrentFigure == 0)
    {
        iCurrentFigure = createNewFigureWithAxes();
    }

    setGraphicObjectProperty(iCurrentFigure, __GO_NAME__, l1, jni_string, 1);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    freeAllocatedSingleString(l1);

    return 0;
}

/*--------------------------------------------------------------------------*/
