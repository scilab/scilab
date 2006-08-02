/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_barrier.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_barrier, fin = 6
 ******************************************/
int intspvm_barrier(char *fname)
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable count */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(3,"i",&un,&un,&l3);/* named: res */
  *istk(l3) = pvm_barrier(cstk(l1),*istk(l2));
  LhsVar(1)= 3;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
