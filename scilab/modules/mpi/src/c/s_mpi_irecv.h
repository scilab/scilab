/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include <stdio.h>

/**
 * TODO : comment
 *
 *
 */
void C2F(s_mpi_irecv)(const int * const source,  /* INPUT */
		 const int * const tag,     /* INPUT */
		 const int * const comm,    /* INPUT */
		 int *request, int *res);    /* OUTPUT */
