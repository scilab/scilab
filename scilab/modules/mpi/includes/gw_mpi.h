/**
 * Gateway header file (declaration of the "links" 
 * between Scilab and the MPI "library"
 * @author Sylvestre LEDRU INRIA 2007
 */

#ifndef __GW_MPI__
#define __GW_MPI__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>
#include "machine.h"
#include "stack-c.h"

int gw_mpi(void);

typedef int (*MPI_Interf) (char *fname,unsigned long l);

typedef struct table_struct
{
  MPI_Interf f;    /** function **/
  char *name;      /** its name **/
} MPITable;

// Declaration of all the profile function declared and 
// used in sci_gateway/*/
int sci_mpi_init (char *fname,unsigned long fname_len);
int sci_mpi_finalize (char *fname,unsigned long fname_len);
int sci_mpi_comm_size (char *fname,unsigned long fname_len);
int sci_mpi_comm_rank (char *fname,unsigned long fname_len);
int sci_mpi_recv (char *fname,unsigned long fname_len);
int sci_mpi_send (char *fname,unsigned long fname_len);

/*
int intsmpi_comm_size (char *fname,unsigned long fname_len));

int intsmpi_irecv (char *fname,unsigned long fname_len));
int intsmpi_isend (char *fname,unsigned long fname_len));
*/
#endif /*  __GW_MPI__ */

