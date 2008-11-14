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
 * SCILAB function : pvm_error
 ******************************************/
int intspvm_error(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mres,nres=1;
  char *res; 
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable func */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  res = scipvm_error_msg(*istk(l1));
  CreateVarFromPtr(Rhs+2,STRING_DATATYPE,(mres=(int)strlen(res), &mres), &nres, &res);
  LhsVar(1)=Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
