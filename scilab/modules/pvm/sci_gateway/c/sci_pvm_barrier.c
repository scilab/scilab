/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_barrier, fin = 6
 ******************************************/
int intspvm_barrier _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  /*  checking variable count */
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(Rhs+3,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l3);/* named: res */
  *istk(l3) = pvm_barrier(cstk(l1),*istk(l2));

  LhsVar(1)= Rhs+3;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
