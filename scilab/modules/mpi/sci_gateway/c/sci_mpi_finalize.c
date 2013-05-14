/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
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
#include "Scierror.h"

/******************************************
 * SCILAB function : mpi_finalize, fin = 2
 ******************************************/

static void mpi_finalize_internal()
{
    free(listRequestPointer);
    free(listRequestPointerSize);
    free(request);
}

int sci_mpi_finalize(char *fname, unsigned long fname_len)
{
    int iRet;
    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);
    mpi_finalize_internal();
    iRet = MPI_Finalize();
    if (iRet != MPI_SUCCESS)
    {
        printf("ici %d\n", iRet);
        char error_string[MPI_MAX_ERROR_STRING];
        int length_of_error_string;

        MPI_Error_string(iRet, error_string, &length_of_error_string);
        Scierror(999, "%s: Could not finalize the MPI instance: %s\n", fname, error_string);
        return 1;
    }

    // TODO: catch les erreurs
    AssignOutputVariable(pvApiCtx, 1) = 0;
    C2F(putlhsvar) ();

    return 0;

}
