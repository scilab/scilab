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
 * SCILAB function : pvm_joingroup, fin = 1
 ******************************************/
int intspvm_joingroup _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(Rhs+2,"i",&un,&un,&l2);/* named: inum */
  *istk(l2) = pvm_joingroup(cstk(l1));
  LhsVar(1)= Rhs+2;
  pvm_error_check(fname,*istk(l2),fname_len);
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
