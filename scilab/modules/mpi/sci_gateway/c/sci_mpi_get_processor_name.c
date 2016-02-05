/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20011 - DIGITEO - Sylvestre LEDRU
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
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

/**
 * This function returns the rank of a process
 */
int sci_mpi_get_processor_name(char *fname, void* pvApiCtx)
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
