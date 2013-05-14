/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 * SCILAB function : mpi_get_processor_name, fin = 3
 * This function returns the rank of a process
 */
int sci_mpi_get_processor_name(char *fname, unsigned long fname_len)
{
    MPI_Comm comm = NULL;
    int *piAddr = NULL;
    int iRows = 1;
    int iCols = 1;
    int iRows2 = 1;
    int iCols2 = 1;
    int iSizeProcessorName;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);            // The output of the function (1 parameter)

    MPI_Get_processor_name(processorName, &iSizeProcessorName);

    int iRet = createSingleString(pvApiCtx, Rhs + 1, processorName);

    if (iRet)
    {
        // TODO: update error message
        Scierror(999, "error in the creation of the variable");
    }

    AssignOutputVariable(pvApiCtx, 1) = Rhs + 1;
    C2F(putlhsvar) ();
    return 0;
}
