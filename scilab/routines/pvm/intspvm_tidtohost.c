/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_tidtohost.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_tidtohost, fin = 13
 ******************************************/
int intspvm_tidtohost(char *fname)
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tid */
  GetRhsVar(1,"i",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  /* cross variable size checking */
  CreateVar(2,"i",&un,&un,&l2);/* named: res */
  *istk(l2) = pvm_tidtohost(*istk(l1));
  LhsVar(1)= 2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 

