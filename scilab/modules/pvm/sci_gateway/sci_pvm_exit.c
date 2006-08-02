/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_exit.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_exit, fin = 17
 ******************************************/
int intspvm_exit(char *fname)
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  *istk(l1) = pvm_exit();
  LhsVar(1)= 1;
  pvm_error_check(fname,*istk(l1));
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
