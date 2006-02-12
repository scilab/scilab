/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_mytid.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_mytid, fin = 16
 ******************************************/
int intspvm_mytid(char *fname)
{
  int un=1,l1;
  CheckRhs(0,0);
  CheckLhs(1,1);
  /* cross variable size checking */
  CreateVar(1,"i",&un,&un,&l1);/* named: res */
  *istk(l1) = pvm_mytid();
  LhsVar(1)= 1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
