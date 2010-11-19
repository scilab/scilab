/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/**************************************************
 * An example of an hand written interface 
 * We call a scilab function (disp) inside the interface 
 **************************************************/

int intex7c(char *fname) 
{
  int mlhs,mrhs,ibegin;
  static int l1, m1, n1, m2, n2, l2 ; 
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  static char name[] = "disp" ;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);

  /* 
   *  To call a function it is required that its input arguments are
   *  stored in the last positions of the variables stack (it is the
   *  case here. NOTE that when 
   *  called, the function destroys its input variables and replaces them by 
   *  the output variables. 
   *  Here  function  takes  variables 1 and 2 as inputs and generates output
   *  variables at positions 1.
   *  ibegin must be the index of the first input variable of a_function
   */
  ibegin=1;

  /* execute the Scilab disp function */
  mlhs=1; mrhs=2; 
  SciString(&ibegin,name,&mlhs,&mrhs);
  /* check if an error has occurred while running a_function */
  if (Err > 0 ) return 0;

  /*  Note that disp, as every function which has nothing to return,
   *  creates as output a variable with special type 0.
   */

  LhsVar(1) = 0; 
  return 0;
}

