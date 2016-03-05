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

/*--------------------------------------------------------------------------*/
#include <stdlib.h>

#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "api_scilab.h"
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
int sci_getcallbackobject(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrpObjUID = NULL;
    int nbRow = 0;
    int nbCol = 0;
    double ObjUID = 0;
    unsigned long graphicHandle = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrpObjUID);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getScalarDouble(pvApiCtx, piAddrpObjUID, &ObjUID))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real expected.\n"), fname, 1);
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return FALSE;
    }

    graphicHandle = getHandle((int)ObjUID);

    /* Create return variable */
    if (graphicHandle == 0)     /* Non-existing object --> return [] */
    {
        if (createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    else                        /* Return the handle */
    {
        if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, graphicHandle))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
