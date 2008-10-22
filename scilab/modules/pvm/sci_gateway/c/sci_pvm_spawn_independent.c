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
/* PVM functions interfaces */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_spawn_independent, fin = 20
 ******************************************/
int intspvm_spawn_independent(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mn1,m2,n2,l2,un=1,m3,n3,l3,lwhere,tids,res;
  static char def_where[]="null"; 
  char *where; 
  CheckRhs(2,3);
  CheckLhs(1,2);
  /*  checking variable task */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  mn1=m1*n1;
  /*  checking variable ntask */
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  where = def_where; 
  if ( Rhs >= 3 ) { 
    GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3); 
    if ( m3*n3 != 0 ) 
      where = cstk(l3); 
  }
  lwhere=strlen(where);
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&un,istk(l2),&tids);/* named: tids */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&res);/* named: res */
  C2F(scipvmspawnindependent)(cstk(l1),&mn1,istk(l2),where,&lwhere,istk(tids),istk(res));
  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;

  pvm_error_check(fname,*istk(res),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/ 
