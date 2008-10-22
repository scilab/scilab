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
 * SCILAB function : pvm_bufinfo
 ******************************************/
int intspvm_bufinfo(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,un=1;
  int bytes,msgtag,tid,res,i;
  CheckRhs(1,1);
  CheckLhs(1,4);
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);/* bufid */ 
  CheckScalar(1,m1,n1);
  CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&bytes);
  CreateVar(Rhs+3,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&msgtag);
  CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&tid);
  CreateVar(Rhs+5,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&res);
  C2F(scipvmbufinfo)(istk(l1), istk(bytes), istk(msgtag), istk(tid), istk(res)); 

  for ( i = 1 ; i <= Lhs ; i++) LhsVar(i)= Rhs+i+1; 

  pvm_error_check(fname,*istk(res),fname_len);
  C2F(putlhsvar)();
  return 0;  
}
/*--------------------------------------------------------------------------*/
