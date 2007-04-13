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
 * SCILAB function : pvm_bufinfo
 ******************************************/
int intspvm_bufinfo _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,un=1;
  int bytes,msgtag,tid,res,i;
  CheckRhs(1,1);
  CheckLhs(1,4);
  GetRhsVar(1,"i",&m1,&n1,&l1);/* bufid */ 
  CheckScalar(1,m1,n1);
  CreateVar(Rhs+2,"i",&un,&un,&bytes);
  CreateVar(Rhs+3,"i",&un,&un,&msgtag);
  CreateVar(Rhs+4,"i",&un,&un,&tid);
  CreateVar(Rhs+5,"i",&un,&un,&res);
  C2F(scipvmbufinfo)(istk(l1), istk(bytes), istk(msgtag), istk(tid), istk(res)); 
  for ( i = 1 ; i <= Lhs ; i++) 
    LhsVar(i)= Rhs+i+1; 
  pvm_error_check(fname,*istk(res),fname_len);
  return 0;  
}
/*-----------------------------------------------------------------------------------*/
