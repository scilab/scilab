/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 * Shows how to pass 
 *       - Scilab boolean matrices as arguments 
 *       how to create and return new boolean matrices
 **************************************************/

/*-------------------------------------------------
 * GetRhsVar(..,'b',...) and CreateVar(..,'b',...
 *-------------------------------------------------*/

static void not (int * iv1,int *iv2,int  size);

int intex3c(char *fname) 
{
  static int l1,m1, n1,l2;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* a boolean matrix argument */ 
  
  GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &l1);

  /* create a new boolean matrix same size as first argument 
   *
   * note that this is only for illustration since we could directly work 
   * with istk(l1) which can be modified and returned. 
   */

  CreateVar(2,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l2);

  /* fills new matrix  */
  
  not(istk(l2),istk(l1),m1*n1);

  LhsVar(1) = 2; /* returns new matrix */
  return 0;
}


static void not(int *iv1,int *iv2,int size)
{
  int i;
  for ( i=0 ; i < size ; i++) iv1[i] = ! iv2[i];
}
