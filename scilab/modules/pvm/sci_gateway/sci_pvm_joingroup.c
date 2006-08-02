/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_joingroup.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_joingroup, fin = 1
 ******************************************/
int intspvm_joingroup(char *fname)
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: inum */
  *istk(l2) = pvm_joingroup(cstk(l1));
  LhsVar(1)= 2;
  pvm_error_check(fname,*istk(l2));
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
