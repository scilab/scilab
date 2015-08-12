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
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "deserialization.h"

int sci_mpi_recv(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piBuffer = NULL;
    int iBufferSize = 0;

    int *piAddr1 = NULL;
    int *piAddr2 = NULL;
    double Tag = 0;
    double Rank = 0;
    MPI_Status status;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

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

    //wait message "Rank" node
    iRet = MPI_Probe((int)Rank, (int)Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Probe failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    //get data size
    iRet = MPI_Get_count(&status, MPI_INT, &iBufferSize);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Get_count failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    //alloc memory to receive data
    piBuffer = (int *)MALLOC(sizeof(int) * iBufferSize);
    if (piBuffer == NULL)
    {
        Scierror(999, _("%s: Could not create the received variable.\n"), fname);
        return 0;
    }

    //receive data
    iRet = MPI_Recv(piBuffer, iBufferSize, MPI_INT, (int)Rank, (int)Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: MPI_Recv failed. Rank %d / Tag %d: %s\n"), fname, Rank, Tag, error_string);
        return 0;
    }

    //convert data from MPI to Scilab
    iRet = deserialize_from_mpi(pvApiCtx, piBuffer, iBufferSize);
    FREE(piBuffer);
    if (iRet)
    {
        Scierror(999, _("%s: Unable to deserialize data !\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

