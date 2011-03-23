/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/* checks if all entries of a vector are finite */
#include "machine.h"
#include "core_math.h"
#include "vfinite.h"
#include "finite.h"


int C2F(vfinite)(int *n, double *v)
{
  int i;
  for (i=0; i < *n; i++) 
    if (finite(v[i])==0) return 0;
  return 1;
}

int C2F(vfiniteComplex)(int *n, doublecomplex *v)
{
  int i;
  for (i=0; i < *n; i++) 
    if (finiteComplex(v[i])==0) return 0;
  return 1;
}
