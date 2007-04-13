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
 * SCILAB function : pvm_set_timer, fin = 14
 ******************************************/
int intspvm_set_timer _PARAMS((char *fname,unsigned long fname_len))
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(Rhs+1,"i",&un,&un,&l1);/* named: res */
  C2F(scipvmsettimer)(istk(l1));
  LhsVar(1)= Rhs+1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 

