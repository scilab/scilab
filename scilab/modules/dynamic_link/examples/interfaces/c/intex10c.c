/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *     1- Accessing a Scilab Matrix (read and write) 
 *        by its name. 
 *****************************************/

int intex10c(char* fname)
{ 
  int l1;
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  static int m, n, lp;
  int k;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /*
   * we want to access scilab variable : param 
   * we can modify or read param with *stk(lp+k)
   */ 

  GetMatrixptr("param", &m, &n, &lp);
  CreateVar(1,MATRIX_OF_DOUBLE_DATATYPE,  &m, &n, &l1);

  for (k = 0; k < m*n ; ++k) 
    { 
      (*stk(l1+k)) = (*stk(lp+k));
    }
  
  LhsVar(1) = 1;
  return(0);
}

