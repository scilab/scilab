/* Copyright INRIA */
#include "../machine.h"

/* ip is a pointer to a FORTRAN variable coming from SCILAB
which is itself a pointer to an array of n doubles typically
coming from a C function
   cdoublef converts this double array into a double array in op 
   moreover, pointer ip is freed */

void C2F(cdoublef)(n,ip,op)
int *n;
double *ip[];
double *op;
{
  int i;
  if ( *n >0 ) {
      for (i = 0; i < *n; i++)
	  op[i]=(*ip)[i];
      free((char *)(*ip));
  }
}
