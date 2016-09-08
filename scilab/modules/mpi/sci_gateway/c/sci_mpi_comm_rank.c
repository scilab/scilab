/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
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
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "getOptionalComm.h"

/**
 * This function returns the rank of a process within the specified communicator.
 */
int sci_mpi_comm_rank(char *fname, void* pvApiCtx)
{
    int comm_rank = -1;

    CheckInputArgument(pvApiCtx, 0, 1); // Check the parameters of the function ... Here 0 or 1
    CheckOutputArgument(pvApiCtx, 1, 1); // The output of the function (1 parameter)

    // return the communicator from optional argument "comm"
    // if no optional "comm" is given, return MPI_COMM_WORLD
    MPI_Comm comm = getOptionalComm(pvApiCtx);
    if (comm == NULL)
    {
        Scierror(999, _("%s: Wrong type for input argument #%s: An MPI communicator expected.\n"), fname, "comm");
        return 0;
    }

    if (comm != MPI_COMM_NULL)
    {
        MPI_Comm_rank(comm, &comm_rank);
    }

    if (createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, (double)comm_rank))
    {
        Scierror(999, _("%s: Unable to create variable.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

