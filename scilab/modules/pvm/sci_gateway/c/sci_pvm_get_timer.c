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
 * SCILAB function : pvm_get_timer, fin = 15
 ******************************************/
int intspvm_get_timer _PARAMS((char *fname,unsigned long fname_len))
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&l1);/* named: res */
  C2F(scipvmgettimer)(stk(l1));
  LhsVar(1)= Rhs+1;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
