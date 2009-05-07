/**
 * @author INRIA Lorraine - 1999
 * @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> INRIA Rocquencourt - 2007
 */
#include <stdio.h>


void C2F(s_mpi_send)(int *pack, int *n,
		double *buff,
		int *dest,
		int *tag,
		int *comm,
					 int *res);
