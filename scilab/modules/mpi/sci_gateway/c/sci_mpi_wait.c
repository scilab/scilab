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


int sci_mpi_wait(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int iType = 0;
    int iRows = 0;
    int iCols = 0;
    int *piBuffer = NULL;
    int iBufferSize = 0;
    double NodeID = 0;
    double RequestID;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddr, &RequestID))
    {
        return 1;
    }

    if (RequestID < 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive value expected.\n"), fname, 1);
        return 0;
    }

    iRet = MPI_Wait(&request[(int)RequestID], MPI_STATUS_IGNORE); /* TODO: MPI_COMM_WORLD should be changed */
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror("%s: Failed to wait for the \n", fname, NodeID, error_string);
        return 1;
    }

    /* Restore the list */
    piBuffer = listRequestPointer[(int)RequestID];
    iBufferSize = listRequestPointerSize[(int)RequestID];
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


    /* free(piBuffer); */
    /* if (createScalarDouble(pvApiCtx, 1, !iRet)) */
    /* { */
    /*     return 1; */
    /* } */

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
