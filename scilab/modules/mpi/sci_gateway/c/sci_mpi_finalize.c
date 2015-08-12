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

#include <stdio.h>
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "MALLOC.h"

static void mpi_finalize_internal()
{
    FREE(listRequestPointer);
    FREE(listRequestPointerSize);
    FREE(request);
}

int sci_mpi_finalize(char *fname, unsigned long fname_len)
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
