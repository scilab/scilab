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
 * SCILAB function : pvm_recv, fin = 21
 ******************************************/
int intspvm_recv(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2,un=1,l3,sizeUsed,size,l5,count=5,l,tid,tag;
  CheckRhs(2,2);
  CheckLhs(1,4);
  /*  checking variable tid */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  tid=*istk(l1);
  /*  checking variable tag */
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  tag=*istk(l2);
  /* space for info */ 
  CreateVar(3,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l3);/* named: res */
  /* space for paking info */ 
  CreateWork(4,&size,&l5); 
  /* variable at position 4 is created as a scalar matrix 
   * but we transmit stk(l5-2) to scipvmrecv which 
   * then can change the header of object at position 4 
   */ 
  C2F(scipvmrecv)(stk(l5-2),&size,&sizeUsed,&tid,&tag,istk(l3));
  /* now we know the exact size used */
  SetWorkSize(4,&sizeUsed);
  LhsVar(1)=4;
  LhsVar(2)=3;
  LhsVar(3)=1;
  LhsVar(4)=2;
  /* since 1 and 2 could be ref */ 
  if ( IsRef(1) ) { 
    CreateVar(count,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l);
    *istk(l)=tid;
    LhsVar(3)=count;
    count++;
  }
  if ( IsRef(2) ) { 
    CreateVar(count,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l);
    *istk(l)=tag;
    LhsVar(4)=count;
  }
  C2F(putlhsvar)();

  pvm_error_check(fname,*istk(l3),fname_len);
  return 0;
}
/*--------------------------------------------------------------------------*/ 
