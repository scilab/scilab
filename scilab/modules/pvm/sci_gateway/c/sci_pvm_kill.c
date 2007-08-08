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
 * SCILAB function : pvm_kill, fin = 18
 ******************************************/
int intspvm_kill _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,mn1,un=1,mn2,l2,i;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,(un=1,&un),(mn2=n1,&mn2),&l2);/* named: res */
  for (i = 0; i < n1; ++i) {
    istk(l2)[i] = pvm_kill( istk(l1)[i]);
    pvm_error_check(fname,istk(l2)[i],fname_len);
  }
  LhsVar(1)= Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
