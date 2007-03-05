/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_pvm_start.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_start 
 ******************************************/
int intspvm_start(char *fname)
{
  int m1,n1,l1,mn1,un=1,res;
  char *host; 
  static char hdefault[]="null";
  CheckRhs(0,1);
  CheckLhs(1,1);
  if (Rhs ==1 ) {
    GetRhsVar(1,"c",&m1,&n1,&l1);mn1=m1*n1;
    host = cstk(l1);
  } else {
    host = hdefault;
    mn1= strlen(hdefault);
  }
  CreateVar(Rhs+1,"i",&un,&un,&res);
  C2F(scipvmstart)(istk(res),host,&mn1);
  LhsVar(1)=Rhs+1;
  pvm_error_check(fname,*istk(res));
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
