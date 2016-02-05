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

/**
 * This function returns the rank of a process within
 * the specified communicator.
 */
int sci_mpi_comm_rank(char *fname, void* pvApiCtx)
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

