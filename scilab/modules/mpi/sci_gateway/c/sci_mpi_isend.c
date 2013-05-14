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
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "serialization.h"

#define TAG 0


int sci_mpi_isend(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int *piAddr3 = NULL;
    int iType = 0;

    int *piBuffer = NULL;
    int iBufferSize = 0;
    double NodeID = 0;
    double RequestID = 0;

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr2, &NodeID))
    {
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr3, &RequestID))
    {
        return 1;
    }

    if (RequestID < 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive value expected.\n"), fname, 3);
        return 0;
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
    }

    iRet = MPI_Isend(piBuffer, iBufferSize, MPI_INT, NodeID, TAG, MPI_COMM_WORLD, &request[(int)RequestID]);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror("%s: Could not send the variable to the node %d: %s\n", fname, NodeID, error_string);
        return 1;
    }

    free(piBuffer);
    if (createScalarBoolean(pvApiCtx, 1, !iRet))
    {
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
