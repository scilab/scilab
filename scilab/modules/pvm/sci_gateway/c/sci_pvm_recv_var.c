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
 * SCILAB function : pvm_recv_var, fin = 24
 ******************************************/
int intspvm_recv_var _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /*  checking variable tag */
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /*  checking variable buff */
  GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
  /* cross variable size checking */
  CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l4);/* named: res */
  C2F(scipvmrecvvar)(istk(l1),istk(l2),cstk(l3),istk(l4));
  LhsVar(1)= Rhs+4;
  
  pvm_error_check(fname,*istk(l4),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
