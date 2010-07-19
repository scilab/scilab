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
 * SCILAB function : pvm_kill, fin = 18
 ******************************************/
int sci_pvm_kill(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mn1,un=1,mn2,l2,i;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,(un=1,&un),(mn2=n1,&mn2),&l2);/* named: res */
  for (i = 0; i < n1; ++i) {
    istk(l2)[i] = pvm_kill( istk(l1)[i]);
    pvm_error_check(fname,istk(l2)[i],fname_len);
  }
  LhsVar(1)= Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
