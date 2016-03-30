/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2011 - DIGITEO - Sylvestre LEDRU
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
