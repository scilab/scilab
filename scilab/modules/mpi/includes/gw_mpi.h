/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_MPI__
#define __GW_MPI__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>
#include "machine.h"
#include "api_scilab.h"
#include "stack-c.h"

int gw_mpi(void);

typedef int (*MPI_Interf) (char *fname, unsigned long l);

typedef struct table_struct
{
    MPI_Interf f;  /** function **/
    char *name;    /** its name **/
} MPITable;

// Declaration of all the profile function declared and
// used in sci_gateway
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

/*
int intsmpi_comm_size (char *fname,unsigned long fname_len));

int intsmpi_irecv (char *fname,unsigned long fname_len));
int intsmpi_isend (char *fname,unsigned long fname_len));
*/
#endif /*  __GW_MPI__ */
