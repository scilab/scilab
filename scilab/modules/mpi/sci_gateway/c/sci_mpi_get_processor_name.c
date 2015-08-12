/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

/**
 * This function returns the rank of a process
 */
int sci_mpi_get_processor_name(char *fname, unsigned long fname_len)
{
    int iSizeProcessorName;
    char processorName[MPI_MAX_PROCESSOR_NAME];
    int iRet = 0;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);            // The output of the function (1 parameter)

    iRet = MPI_Get_processor_name(processorName, &iSizeProcessorName);
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not get processor name: %s\n"), fname, error_string);
        return 0;
    }

    if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, processorName))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
