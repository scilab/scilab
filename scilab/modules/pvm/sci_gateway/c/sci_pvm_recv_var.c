/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_recv_var.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_recv_var, fin = 24
 ******************************************/
int intspvm_recv_var(char *fname)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /*  checking variable tag */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /*  checking variable buff */
  GetRhsVar(3,"c",&m3,&n3,&l3);
  /* cross variable size checking */
  CreateVar(4,"i",&un,&un,&l4);/* named: res */
  C2F(scipvmrecvvar)(istk(l1),istk(l2),cstk(l3),istk(l4));
  LhsVar(1)= 4;
  pvm_error_check(fname,*istk(l4));
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
