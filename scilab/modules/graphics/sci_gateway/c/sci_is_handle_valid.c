/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
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
/* file: sci_is_handle_valid.h                                            */
/* desc : interface for xaxis routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "HandleManagement.h"
#include "Scierror.h"
#include "localization.h"
#include "getPropertyAssignedValue.h"

/*--------------------------------------------------------------------------*/
int sci_is_handle_valid(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrhandleStackPointer = NULL;
    long long* handleStackPointer = NULL;
    int* resultStackPointer       = NULL;

    /* Call isValid = is_handle_valid(h) with h a matrix of handle */
    /* and isValid a matrix of boolean */

    int nbCol = 0;
    int nbRow = 0;
    int nbHandle = 0;
    int i = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* Get handles matrix */
    if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
    {
        Scierror(200, _("%s: Wrong type for input argument #%d: Matrix of handle expected.\n"), fname, 1);
        return  -1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrhandleStackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrhandleStackPointer, &nbRow, &nbCol, &handleStackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(200, _("%s: Wrong type for input argument #%d: Matrix of handle expected.\n"), fname, 1);
        return 1;
    }

    nbHandle = nbRow * nbCol;

    /* create output matrix */
    allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nbRow, nbCol, &resultStackPointer);

    /* Check each handle */
    for (i = 0; i < nbHandle; i++)
    {
        resultStackPointer[i] =  (getObjectFromHandle((long int)handleStackPointer[i]) != 0 ? TRUE : FALSE);
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
