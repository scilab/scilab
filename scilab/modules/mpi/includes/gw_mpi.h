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

MPI_IMPEXP int gw_mpi(void);

typedef int (*MPI_Interf) (char *fname, unsigned long l);
/*--------------------------------------------------------------------------*/
/* interface for the previous function Table */
/*--------------------------------------------------------------------------*/
int sci_mpi_init(char *fname, unsigned long fname_len);
int sci_mpi_finalize(char *fname, unsigned long fname_len);
int sci_mpi_comm_size(char *fname, unsigned long fname_len);
int sci_mpi_comm_rank(char *fname, unsigned long fname_len);
int sci_mpi_recv(char *fname, unsigned long fname_len);
int sci_mpi_send(char *fname, unsigned long fname_len);
int sci_mpi_get_processor_name(char *fname, unsigned long fname_len);
int sci_mpi_bcast(char *fname, unsigned long fname_len);
int sci_mpi_irecv(char *fname, unsigned long fname_len);
int sci_mpi_isend(char *fname, unsigned long fname_len);
int sci_mpi_wait(char *fname, unsigned long fname_len);
#endif /*  __GW_MPI__ */
