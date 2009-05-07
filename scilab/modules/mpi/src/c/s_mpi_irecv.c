/**
 * Copyright (c) 1999 - INRIA Lorraine
 * Copyright (c) 2007 - Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt
 */

#include "sci_mpi.h"
#include "s_mpi_irecv.h"

void C2F(s_mpi_irecv)(const int * const source,  /* INPUT */
		 const int * const tag,     /* INPUT */
		 const int * const comm,    /* INPUT */
		 int *request, int *res)    /* OUTPUT */
{
	*res = MPI_SUCCESS;
} /* scimpiirecv */
