/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <mpi.h>
#include "MALLOC.h"

int pre_send_metadata(int size, MPI_Datatype dataType, int dest, int tag)
{
    /* Find out the buffer size */
    int mpi_buffsize = (1 * sizeof(int)) + (1 * sizeof(MPI_Datatype));
    char *mpi_buff = (char *) MALLOC(mpi_buffsize * sizeof(char));
    int position = 0;

    /* Serialize both size and type to send it the slave */
    MPI_Pack(&size, 1, MPI_INT, mpi_buff, mpi_buffsize, &position, MPI_COMM_WORLD);
    MPI_Pack(&dataType, 1, MPI_INT, mpi_buff, mpi_buffsize, &position, MPI_COMM_WORLD);
    MPI_Send(mpi_buff, position, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
    /* TODO: check this result */
    FREE(mpi_buff);
    return 0;
}

int pre_recv_metadata(int source, int tag, int *size, MPI_Datatype *dataType)
{
    int position = 0;
    MPI_Status stat;
    int mpi_buffsize = (1 * sizeof(int)) + (1 * sizeof(MPI_Datatype));
    char *mpi_buff = (char *) MALLOC(mpi_buffsize * sizeof(char));

    /* DeSerialize both size and type from the master */
    MPI_Recv(mpi_buff, mpi_buffsize, MPI_PACKED, source, tag, MPI_COMM_WORLD, &stat);
    /* TODO: check stat */
    MPI_Unpack(mpi_buff, mpi_buffsize, &position, &size, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(mpi_buff, mpi_buffsize, &position, &dataType, 1, MPI_INT, MPI_COMM_WORLD);
    return 0;
}
