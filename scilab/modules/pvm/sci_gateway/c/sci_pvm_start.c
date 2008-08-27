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
 * SCILAB function : pvm_start 
 ******************************************/
int intspvm_start(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,mn1,un=1,res;
  char *host; 
  static char hostdefault[]="null";
  CheckRhs(0,1);
  CheckLhs(1,1);
  if (Rhs ==1 ) {
    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	mn1=m1*n1;
    host = cstk(l1);
  } else {
	  /* Don't have any argument, then load the default host */
    host = hostdefault;
    mn1= strlen(hostdefault);
  }
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&res);
  C2F(scipvmstart)(istk(res),host,&mn1);
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();
  pvm_error_check(fname,*istk(res),fname_len);
  return 0;
}
/*--------------------------------------------------------------------------*/ 
