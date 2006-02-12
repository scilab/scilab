/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_set_timer.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_set_timer, fin = 14
 ******************************************/
int intspvm_set_timer(char *fname)
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  C2F(scipvmsettimer)(istk(l1));
  LhsVar(1)= 1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 