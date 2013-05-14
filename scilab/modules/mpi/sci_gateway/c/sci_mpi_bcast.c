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
#include "localization.h"
#include "serialization.h"
#include "deserialization.h"

int sci_mpi_bcast(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int iType = 0;

    int *piBuffer = NULL;
    int iBufferSize = 0;
    double rootID = 0;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &rootID))
    {
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    switch (iType)
    {
        case sci_matrix:
            iRet = serialize_double(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
            break;
        case sci_strings:
            iRet = serialize_string(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
            break;
        case sci_boolean:
            iRet = serialize_boolean(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
            break;
        case sci_sparse:
            iRet = serialize_sparse(pvApiCtx, piAddr, &piBuffer, &iBufferSize, TRUE);
            break;
        case sci_boolean_sparse:
            iRet = serialize_sparse(pvApiCtx, piAddr, &piBuffer, &iBufferSize, FALSE);
            break;
        case sci_ints:
            iRet = serialize_int(pvApiCtx, piAddr, &piBuffer, &iBufferSize);
            break;
        default:
            Scierror(999, _("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"), fname, iType);
            break;
    }

    if (iRet)
    {
        printf("pas reussi a seraliser\n");
    }
    int rank, length;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* First, send the size of the data as broadcast */
    iRet = MPI_Bcast(&iBufferSize, 1, MPI_INT, rootID, MPI_COMM_WORLD);

    /* Second, restore the data with the right size */
    iRet = MPI_Bcast(piBuffer, iBufferSize, MPI_INT, rootID, MPI_COMM_WORLD);

    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        printf("ICI \n");
        fflush(NULL);
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, "%s: Could not broadcast the variable to the node %d: %s\n", fname, rootID, error_string);
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

    //    free(piBuffer);

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
