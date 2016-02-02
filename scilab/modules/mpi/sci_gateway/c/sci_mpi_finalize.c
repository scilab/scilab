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

#include <stdio.h>
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "sci_malloc.h"

static void mpi_finalize_internal()
{
    FREE(listRequestPointer);
    FREE(listRequestPointerSize);
    FREE(request);
}

int sci_mpi_finalize(char *fname, void* pvApiCtx)
{
    int iRet = 0;
    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);
    mpi_finalize_internal();
    iRet = MPI_Finalize();
    if (iRet != MPI_SUCCESS)
    {
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;
        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, _("%s: Could not finalize the MPI instance: %s\n"), fname, error_string);
        return 0;
    }

    // TODO: catch les erreurs
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;

}
