/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_halt.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_halt 
 ******************************************/
int intspvm_halt(char *fname)
{
  int res,un=1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVar(1,"i",&un,&un,&res);
  C2F(scipvmhalt)(istk(res));
  LhsVar(1)=1;
  pvm_error_check(fname,*istk(res));
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
