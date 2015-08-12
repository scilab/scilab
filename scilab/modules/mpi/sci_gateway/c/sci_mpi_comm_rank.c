/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"

/**
 * This function returns the rank of a process within
 * the specified communicator.
 */
int sci_mpi_comm_rank (char *fname, unsigned long fname_len)
{
    int comm_rank = 0;

    CheckInputArgument(pvApiCtx, 0, 0); // Check the parameters of the function ... Here 0
    CheckOutputArgument(pvApiCtx, 1, 1); // The output of the function (1 parameter)

    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, (double)comm_rank))
    {
        Scierror(999, _("%s: Unable to create variable.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

