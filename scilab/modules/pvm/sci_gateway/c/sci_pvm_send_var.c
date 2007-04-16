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
 * SCILAB function : pvm_send_var, fin = 25
 ******************************************/
int intspvm_send_var _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,mn1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  /*  checking variable buff */
  GetRhsVar(2,"c",&m2,&n2,&l2);
  /*  checking variable tag */
  GetRhsVar(3,"i",&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(Rhs+4,"i",&un,&un,&l4);/* named: res */
  C2F(scipvmsendvar)(istk(l1),&n1,cstk(l2),istk(l3),istk(l4));
  LhsVar(1)= Rhs+4;

  pvm_error_check(fname,*istk(l4),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
