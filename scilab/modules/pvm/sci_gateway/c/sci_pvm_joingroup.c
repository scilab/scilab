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
 * SCILAB function : pvm_joingroup, fin = 1
 ******************************************/
int intspvm_joingroup(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,un=1,l2;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  /* cross variable size checking */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l2);/* named: inum */
  *istk(l2) = pvm_joingroup(cstk(l1));
  LhsVar(1)= Rhs+2;
  
  pvm_error_check(fname,*istk(l2),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
