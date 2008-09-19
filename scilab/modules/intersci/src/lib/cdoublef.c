/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


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
