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
 * SCILAB function : pvm_gsize, fin = 3
 ******************************************/
int intspvm_gsize _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l2);/* named: size */
  *istk(l2) = pvm_gsize(cstk(l1));
  LhsVar(1)= Rhs+2;
  pvm_error_check(fname,*istk(l2),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
