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

#include "api_scilab.h"
#include <stdio.h>
#include <mpi.h>
#include "gw_mpi.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "serialization.h"
#include "deserialization.h"

int sci_mpi_bcast(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;

    int *piBuffer = NULL;
    int iBufferSize = 0;
    double rootID = 0;
    int rank = 0;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &rootID))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    iRet = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string = 0;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not get communicator rank of the node %d: %s\n"), fname, rootID, error_string);
        return 0;
    }

    if (rank == rootID)
    {
        iRet = serialize_to_mpi(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
        if (iRet)
        {
            Scierror(999, _("Unable to serialize data\n"));
            return 0;
        }
    }

    /* First, send the size of the data as broadcast */
    iRet = MPI_Bcast(&iBufferSize, 1, MPI_INT, (int)rootID, MPI_COMM_WORLD);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string = 0;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not broadcast the variable to the node %d: %s\n"), fname, rootID, error_string);
        return 0;
    }

    if (rank != rootID)
    {
        piBuffer = (int*)MALLOC(sizeof(int) * iBufferSize);
    }

    /* Second, restore the data with the right size */
    iRet = MPI_Bcast(piBuffer, iBufferSize, MPI_INT, (int)rootID, MPI_COMM_WORLD);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string = 0;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not broadcast the variable to the node %d: %s\n"), fname, rootID, error_string);
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
