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
#include <stdio.h>
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "deserialization.h"
#include "getOptionalComm.h"

int sci_mpi_irecv(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet = 0;
    int *piBuffer = NULL;
    int iBufferSize = 0;

    int *piAddr1 = NULL;
    int *piAddr2 = NULL;
    int *piAddr3 = NULL;
    double Tag = 0;
    double Rank = 0;
    int iRequestID = 0;
    double dblRequestID = 0;
    MPI_Status status;

    CheckInputArgument(pvApiCtx, 3, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    // if no optional "comm" is given, return MPI_COMM_WORLD
    MPI_Comm comm = getOptionalComm(pvApiCtx);
    if (comm == NULL)
    {
        Scierror(999, _("%s: Wrong type for input argument #%s: An MPI communicator expected.\n"), fname, "comm");
        return 0;
    }

    if (comm == MPI_COMM_NULL)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    //Rank
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr1, &Rank))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 1);
        return 0;
    }

    //Tag
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &Tag))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 2);
        return 0;
    }


    // Request
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr3, &dblRequestID))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), fname, 3);
        return 0;
    }

    iRequestID = (int)dblRequestID;
    if (iRequestID < 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive value expected.\n"), fname, 3);
        return 0;
    }

    iRet = MPI_Probe((int)Rank, (int)Tag, comm, &status);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Probe failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    iRet = MPI_Get_count(&status, MPI_INT, &iBufferSize);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Get_count failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    piBuffer = (int *)MALLOC(sizeof(int) * iBufferSize);
    if (piBuffer == NULL)
    {
        Scierror(999, "%s: Could not create the received variable.\n", fname);
        return 0;
    }

    iRet = MPI_Irecv(piBuffer, iBufferSize, MPI_INT, (int)Rank, (int)Tag, comm, &request[iRequestID]);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Irecv failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    /* Store the pointer piBuffer */
    listRequestPointer[iRequestID] = piBuffer;
    listRequestPointerSize[iRequestID] = iBufferSize;

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

