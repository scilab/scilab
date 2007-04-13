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
 * SCILAB function : pvm_delhosts, fin = 11
 ******************************************/
int intspvm_delhosts _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,un=1,mn2,l2;
  char  **Str1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable hosts */
  GetRhsVar(1,"S",&m1,&n1,&Str1);
  CheckOneDim(1,1,m1,1);
  /* cross variable size checking */
  CreateVar(Rhs+2,"i",(un=1,&un),(mn2=n1,&mn2),&l2);/* named: infos */
  pvm_delhosts(Str1, n1,istk(l2));
  LhsVar(1)= Rhs+2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
