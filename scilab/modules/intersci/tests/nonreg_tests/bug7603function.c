/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "machine.h"

int F2C(ext1c)(int *n, double *a, double *b, double *c)
{
  int k = 0;
  for (k = 0; k < *n; ++k)
      c[k] = a[k] + b[k];
  return(0);
}

