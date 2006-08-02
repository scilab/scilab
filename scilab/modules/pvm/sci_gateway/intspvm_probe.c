/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_probe.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_probe 
 ******************************************/
int intspvm_probe(char *fname)
{
  int m1,n1,l1,m2,n2,l2,un=1;
  int res;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,"i",&m1,&n1,&l1);/*  tid */ 
  CheckScalar(1,m1,n1);
  GetRhsVar(2,"i",&m2,&n2,&l2);/* msgtag */ 
  CheckScalar(2,m2,n2);
  CreateVar(3,"i",&un,&un,&res);
  *istk(res) = pvm_probe(*istk(l1),*istk(l2));
  LhsVar(1)= 3; 
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
