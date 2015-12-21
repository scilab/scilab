/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GW_MPI__
#define __GW_MPI__

#include "dynlib_mpi.h"
#include "c_gateway_prototype.h"

MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_init);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_finalize);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_comm_size);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_comm_rank);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_recv);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_send);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_get_processor_name);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_bcast);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_irecv);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_isend);
MPI_IMPEXP STACK_GATEWAY_PROTOTYPE (sci_mpi_wait);

#endif /*  __GW_MPI__ */
