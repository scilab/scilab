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
 * SCILAB function : pvm_spawn_independent, fin = 20
 ******************************************/
int intspvm_spawn_independent _PARAMS((char *fname,unsigned long fname_len))
{
  int m1,n1,l1,mn1,m2,n2,l2,un=1,m3,n3,l3,lwhere,tids,res;
  static char def_where[]="null"; 
  char *where; 
  CheckRhs(2,3);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  mn1=m1*n1;
  /*  checking variable ntask */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  where = def_where; 
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,"c",&m3,&n3,&l3); 
    if ( m3*n3 != 0 ) 
      where = cstk(l3); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,"i",&un,istk(l2),&tids);/* named: tids */
  CreateVar(Rhs+2,"i",&un,&un,&res);/* named: res */
  C2F(scipvmspawnindependent)(cstk(l1),&mn1,istk(l2),where,&lwhere,istk(tids),istk(res));
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;

  pvm_error_check(fname,*istk(res),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
