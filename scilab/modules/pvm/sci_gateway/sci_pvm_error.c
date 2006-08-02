/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_error.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_error
 ******************************************/
int intspvm_error(char *fname)
{
  int m1,n1,l1,mres,nres=1;
  char *res; 
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable func */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  res = scipvm_error_msg(*istk(l1));
  CreateVarFromPtr(2, "c",(mres=strlen(res), &mres), &nres, &res);
  LhsVar(1)=2; 
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
