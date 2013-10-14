/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int sci_is_handle_valid(char *fname, unsigned long fname_len)
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
