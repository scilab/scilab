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
 * SCILAB function : pvm_getinst, fin = 5
 ******************************************/
int intspvm_getinst _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,m2,n2,l2,un=1,l3;
  CheckRhs(2,2);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable tid */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  /* cross variable size checking */
  CreateVar(Rhs+3,"i",&un,&un,&l3);/* named: inum */
  *istk(l3) = pvm_getinst(cstk(l1),*istk(l2));
  LhsVar(1)= Rhs+3;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
