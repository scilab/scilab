/* Copyright INRIA */
#include "../machine.h"

/* ip is a pointer to a FORTRAN variable coming from SCILAB
which is itself a pointer to an array of n doubles typically
coming from a C function
   cdouble converts this double array into a double array in op */
   
void C2F(cdouble)(n,ip,op)
int *n;
double *ip[];
double *op;
{
  int i;
  for (i = 0; i < *n; i++)
    op[i]=(*ip)[i];
}
