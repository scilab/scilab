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
 * SCILAB function : pvm_tidtohost, fin = 13
 ******************************************/
int intspvm_tidtohost _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /* cross variable size checking */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l2);/* named: res */
  *istk(l2) = pvm_tidtohost(*istk(l1));
  LhsVar(1)= Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 

