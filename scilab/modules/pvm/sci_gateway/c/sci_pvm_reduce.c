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
#include "sci_pvm.h"
#include "gw_pvm.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_reduce 
 ******************************************/
int sci_pvm_reduce(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mn1,m2,n2,l2,m3,n3,l3,un=1,m4,n4,l4,mn4,res;
  int m5,n5,l5;
  CheckRhs(5,5);
  CheckLhs(1,2);
  /*  checking variable func */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);mn1=m1*n1;
  /*  checking variable data */
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  /*  checking variable msgtag */
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /*  checking variable group */
  GetRhsVar(4,STRING_DATATYPE,&m4,&n4,&l4);mn4=m4*n4;
  /*  checking variable rootginst */
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&m5,&n5,&l5);
  CheckScalar(5,m5,n5);
  CreateVar(6,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&res);
  C2F(scipvmreduce)(cstk(l1),&mn1,stk(l2),&m2,&n2,istk(l3),cstk(l4),&mn4,istk(l5),istk(res));
  LhsVar(1)= 2;
  LhsVar(2)= 6;

  pvm_error_check(fname,*istk(res),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/ 
