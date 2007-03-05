/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_get_timer.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_get_timer, fin = 15
 ******************************************/
int intspvm_get_timer(char *fname)
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"d",&un,&un,&l1);/* named: res */
  C2F(scipvmgettimer)(stk(l1));
  LhsVar(1)= 1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
