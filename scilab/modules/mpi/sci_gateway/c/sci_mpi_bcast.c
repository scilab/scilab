/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Sylvestre LEDRU
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

#include "gw_mpi.h"
#include "sci_mpi.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "serialization.h"
#include "deserialization.h"
#include "getOptionalComm.h"

int sci_mpi_bcast(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;

    int *piBuffer = NULL;
    int iBufferSize = 0;
    double rootID = 0;
    int rank = 0;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // return the communicator from optional argument "comm"
    // if no optional "comm" is given, return MPI_COMM_WORLD
    MPI_Comm comm = getOptionalComm(pvApiCtx);
    if (comm == NULL)
    {
        Scierror(999, _("%s: Wrong type for input argument #%s: An MPI communicator expected.\n"), fname, "comm");
        return 0;
    }

    if (comm == MPI_COMM_NULL)
    {
        // return empty matrix
        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0, 0, NULL);
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

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

    iRet = MPI_Comm_rank(comm, &rank);
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
    iRet = MPI_Bcast(&iBufferSize, 1, MPI_INT, (int)rootID, comm);
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
    iRet = MPI_Bcast(piBuffer, iBufferSize, MPI_INT, (int)rootID, comm);
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
