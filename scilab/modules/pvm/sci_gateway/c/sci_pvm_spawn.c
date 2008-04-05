/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * Copyright (C) 2002 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* PVM functions interfaces 
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_spawn, fin = 19
 ******************************************/
int intspvm_spawn (char *fname,unsigned long fname_len)
{
  char *nw, *where ; 
  static char def_nw[]=""; 
  static char def_where[]="null"; 
  int m1,n1,l1,mn1,m2,n2,l2,un=1,lwhere,lnw; 
  int m3,n3,l3,m4,n4,l4,StackTaskId,StackRes;
  CheckRhs(2,4);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  mn1=m1*n1; 
  /*  checking variable ntask */
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  nw = def_nw;
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3); 
    if ( m3*n3 != 0 && strcmp(cstk(l3),"nw")==0) 
      nw = cstk(l3); 
  }
  lnw= strlen(nw);
  where = def_where; 
  if ( Rhs >= 4 ) { 
    GetRhsVar(4,STRING_DATATYPE,&m4,&n4,&l4); 
    if ( m4*n4 != 0 ) 
      where = cstk(l4); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&un,istk(l2),&StackTaskId);/* named: StackTaskId */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&StackRes);/* named: Stackres */
  C2F(scipvmspawn)(cstk(l1),&mn1,nw,&lnw,where,&lwhere,istk(l2),istk(StackTaskId),istk(StackRes));

  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;

  pvm_error_check(fname,*istk(StackRes),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
