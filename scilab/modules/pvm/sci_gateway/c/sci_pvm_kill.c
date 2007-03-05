/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_kill.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_kill, fin = 18
 ******************************************/
int intspvm_kill(char *fname)
{
  int m1,n1,l1,mn1,un=1,mn2,l2,i;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  CreateVar(2,"i",(un=1,&un),(mn2=n1,&mn2),&l2);/* named: res */
  for (i = 0; i < n1; ++i) {
    istk(l2)[i] = pvm_kill( istk(l1)[i]);
    pvm_error_check(fname,istk(l2)[i]);
  }
  LhsVar(1)= 2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
