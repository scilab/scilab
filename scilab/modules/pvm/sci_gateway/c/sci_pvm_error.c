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
 * SCILAB function : pvm_error
 ******************************************/
int intspvm_error _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,mres,nres=1;
  char *res; 
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable func */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  res = scipvm_error_msg(*istk(l1));
  CreateVarFromPtr(Rhs+2,STRING_DATATYPE,(mres=strlen(res), &mres), &nres, &res);
  LhsVar(1)=Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
