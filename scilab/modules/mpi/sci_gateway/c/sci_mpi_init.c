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
#include "api_scilab.h"
#include "stack-c.h"
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "MALLOC.h"

/**
 * SCILAB function : mpi_init, fin = 1
 * This function init the mpi env
 */
MPI_Errhandler errhdl;

/* should be moved elsewhere */
void MPIErrHandler(MPI_Comm * comm, int *errorcode, ...)
{
    char buffer[4096];
    int resultlen;

    MPI_Error_string(*errorcode, buffer, &resultlen);
    buffer[resultlen] = 0;
    printf("Erreur mpi : %s\n", buffer);
}

BOOL mpi_initialized = FALSE;

static void mpi_init_internal()
{
    int i = 0;
    if (mpi_initialized == TRUE)
    {
        return;
    }

    request = (MPI_Request *)MALLOC(REQUEST_MAXSIZE * sizeof(MPI_Request));
    listRequestPointer = (int**)MALLOC(REQUEST_MAXSIZE * sizeof(int*));
    listRequestPointerSize = (int*)MALLOC(REQUEST_MAXSIZE * sizeof(int));
    for (i = 0; i < REQUEST_MAXSIZE; i++)
    {
        request[i] = 0;
        listRequestPointer[i] = NULL;
        listRequestPointerSize[i] = 0;
    }

    mpi_initialized = TRUE;

}

int sci_mpi_init(char *fname, unsigned long fname_len)
{
    int flag;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);
    mpi_init_internal();
    MPI_Initialized(&flag);
    if (!flag)
    {
        /* MPI Not yet initialized */
        MPI_Init(NULL, NULL);
        MPI_Comm_create_errhandler(MPIErrHandler, &errhdl);
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
