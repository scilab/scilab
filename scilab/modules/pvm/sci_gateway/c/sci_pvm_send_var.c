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
 * SCILAB function : pvm_send_var, fin = 25
 ******************************************/
int intspvm_send_var(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mn1,m2,n2,l2,m3,n3,l3,un=1,l4;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  /*  checking variable buff */
  GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
  /*  checking variable tag */
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l4);/* named: res */
  C2F(scipvmsendvar)(istk(l1),&n1,cstk(l2),istk(l3),istk(l4));
  LhsVar(1)= Rhs+4;

  pvm_error_check(fname,*istk(l4),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/ 
