/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int sci_xname(char *fname, unsigned long fname_len)
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
        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
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
