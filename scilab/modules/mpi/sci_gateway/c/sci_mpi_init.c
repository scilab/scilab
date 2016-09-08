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
#include <stdio.h>                      // for NULL, fprintf, stderr
#include <mpi.h>
#include "sci_malloc.h"
#include "BOOL.h"                       // for ::TRUE, ::FALSE, BOOL
#include "api_scilab.h"                 // for pvApiCtx, etc
#include "gw_mpi.h"

/* Create some static datastructure to store all the Request references */
MPI_Request *request;
int **listRequestPointer;
int *listRequestPointerSize;
static int REQUEST_MAXSIZE = 10000;

/**
 * This function init the MPI env
 */
MPI_Errhandler errhdl;

/* should be moved elsewhere */
void MPIErrHandler(MPI_Comm * comm, int *errorcode, ...)
{
    char buffer[4096];
    int resultlen;

    MPI_Error_string(*errorcode, buffer, &resultlen);
    buffer[resultlen] = 0;
    fprintf(stderr, "MPI internal error: %s\n", buffer);
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

int sci_mpi_init(char *fname, void* pvApiCtx)
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
