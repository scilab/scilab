/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "Scierror.h"
#include "MALLOC.h"
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
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr1, &Rank))
    {
        return 1;
    }

    //Tag
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &Tag))
    {
        return 1;
    }

    /*
        iRet = mpi_my_recv(&piBuffer, &iBufferSize);
        if(iRet)
        {
            return 1;
        }
    */

    iRet = MPI_Probe(Rank, Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, "%s: MPI_Probe failed. Rank %d / Tag %d: %s\n", fname, Rank, Tag, error_string);
        return 1;
    }

    if (MPI_Get_count(&status, MPI_INT, &iBufferSize) != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, "%s: MPI_Get_count failed. Rank %d / Tag %d: %s\n", fname, Rank, Tag, error_string);
        return 1;
    }

    piBuffer = (int *)MALLOC(sizeof(int) * iBufferSize);
    if (piBuffer == NULL)
    {
        Scierror(999, "%s: Could not create the received variable.\n", fname);
        return 1;
    }

    iRet = MPI_Recv(piBuffer, iBufferSize, MPI_INT, Rank, Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, "%s: MPI_Recv failed. Rank %d / Tag %d: %s\n", fname, Rank, Tag, error_string);

        return 1;
    }

    switch (piBuffer[0])
    {
        case sci_matrix:
            iRet = deserialize_double(pvApiCtx, piBuffer, iBufferSize);
            break;
        case sci_strings:
            iRet = deserialize_string(pvApiCtx, piBuffer, iBufferSize);
            break;
        case sci_boolean:
            iRet = deserialize_boolean(pvApiCtx, piBuffer, iBufferSize);
            break;
        case sci_sparse:
            iRet = deserialize_sparse(pvApiCtx, piBuffer, iBufferSize, TRUE);
            break;
        case sci_boolean_sparse:
            iRet = deserialize_sparse(pvApiCtx, piBuffer, iBufferSize, FALSE);
            break;
        case sci_ints:
            iRet = deserialize_int(pvApiCtx, piBuffer, iBufferSize);
            break;
        default:
            return 1;
            break;
    }

    free(piBuffer);
    if (iRet)
    {
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

