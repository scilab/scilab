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
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_delhosts, fin = 11
 ******************************************/
int intspvm_delhosts(char *fname,unsigned long fname_len)
{
  int m1,n1,un=1,mn2,l2;
  char  **Str1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  /*  checking variable hosts */
  GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str1);
  CheckOneDim(1,1,m1,1);
  /* cross variable size checking */
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,(un=1,&un),(mn2=n1,&mn2),&l2);/* named: infos */
  pvm_delhosts(Str1, n1,istk(l2));
  freeArrayOfString(Str1,m1);
  LhsVar(1)= Rhs+2;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
