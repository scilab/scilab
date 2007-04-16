/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces 
 * @author  Inria Lorraine - 1997
 * @author Jean-Philippe Chancelier - July 2002
 * @author Allan Cornet INRIA 2006 
 */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_spawn, fin = 19
 ******************************************/
int intspvm_spawn _PARAMS((char *fname,unsigned long fname_len))
{
  char *nw, *where ; 
  static char def_nw[]=""; 
  static char def_where[]="null"; 
  int m1,n1,l1,mn1,m2,n2,l2,un=1,lwhere,lnw; 
  int m3,n3,l3,m4,n4,l4,tids,res;
  CheckRhs(2,4);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  mn1=m1*n1; 
  /*  checking variable ntask */
  GetRhsVar(2,"i",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  nw = def_nw;
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,"c",&m3,&n3,&l3); 
    if ( m3*n3 != 0 && strcmp(cstk(l3),"nw")==0) 
      nw = cstk(l3); 
  }
  lnw= strlen(nw);
  where = def_where; 
  if ( Rhs >= 4 ) { 
    GetRhsVar(4,"c",&m4,&n4,&l4); 
    if ( m4*n4 != 0 ) 
      where = cstk(l4); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,"i",&un,istk(l2),&tids);/* named: tids */
  CreateVar(Rhs+2,"i",&un,&un,&res);/* named: res */
  C2F(scipvmspawn)(cstk(l1),&mn1,nw,&lnw,where,&lwhere,istk(l2),istk(tids),istk(res));
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;

  pvm_error_check(fname,*istk(res),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
