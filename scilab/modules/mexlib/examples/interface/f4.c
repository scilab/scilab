
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

#include <string.h> 
#include <stdio.h>
#include "stack-c.h"
#include "lib/pipo.h"

int int_f4(char *fname) 
{
  static int l1,m1, n1;
  CheckRhs(1,1);
  CheckLhs(1,1); 
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
  CheckScalar(1,m1,n1); 
  *istk(l1)= foo(*istk(l1)); 
  *istk(l1)= bar(*istk(l1)); 
  LhsVar(1) = 1;
  return 0;
}
