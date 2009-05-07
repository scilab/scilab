/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include <stdio.h>
#include <stdlib.h>
#include "build_buff_type.h"
/**
 * TODO : comment
 *
 *
 */
void C2F(s_mpi_recv)(double * const beginvar,   /* I/O */
		const int * const maxsize, /* INPUT */
		int * const size,          /* OUTPUT */
		const int * const source,  /* INPUT */
		const int * const tag,     /* INPUT */
		const int * const comm,    /* INPUT */
		int stat[2], int *res);   /* OUTPUT */
