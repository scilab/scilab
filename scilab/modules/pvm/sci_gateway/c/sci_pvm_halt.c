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
 * SCILAB function : pvm_halt 
 ******************************************/
int intspvm_halt _PARAMS((char *fname,unsigned long fname_len))
{
  int res,un=1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&res);
  C2F(scipvmhalt)(istk(res));
  LhsVar(1)=Rhs+1;

  pvm_error_check(fname,*istk(res),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
