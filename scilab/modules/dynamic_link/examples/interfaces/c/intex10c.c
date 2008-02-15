/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
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
   * we want to acces scilab variable : param 
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

