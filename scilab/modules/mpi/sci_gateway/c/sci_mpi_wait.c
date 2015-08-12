/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "serialization.h"
#include "deserialization.h"

int sci_mpi_wait(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piBuffer = NULL;
    int iBufferSize = 0;
    double NodeID = 0;
    int iRequestID;
    double dblRequestID;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr, &dblRequestID))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 1);
        return 0;
    }

    iRequestID = (int)dblRequestID;

    if (iRequestID < 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive value expected.\n"), fname, 1);
        return 0;
    }

    iRet = MPI_Wait(&request[iRequestID], MPI_STATUS_IGNORE); /* TODO: MPI_COMM_WORLD should be changed */
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Wait failed while waiting %d: %s\n"), fname, NodeID, error_string);
        return 0;
    }

    /* Restore the list */
    piBuffer = listRequestPointer[iRequestID];
    iBufferSize = listRequestPointerSize[iRequestID];

    if (iBufferSize == 0)
    {
        //wait on sender not on receiver
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    iRet = deserialize_from_mpi(pvApiCtx, piBuffer, iBufferSize);
    FREE(piBuffer);
    if (iRet)
    {
        Scierror(999, _("%s: Unable to deserialize data.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
