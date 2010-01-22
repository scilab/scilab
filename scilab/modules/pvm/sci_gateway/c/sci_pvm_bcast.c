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
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_bcast, fin = 7
 ******************************************/
int sci_pvm_bcast(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m3,n3,l3,un=1,l4,size,l5,used,ierr;
  int *Ipack,address,*header;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable group */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  /*  checking variable buff */
  header = GetData(2);
  /*  checking variable msgtag */
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l4);/* named: res */
  /* Use the rest of the stack as working area */ 
  CreateWork(5,&size,&l5); 
  Ipack= (int *) stk(l5);
  /* Build a pack vector for buff: size is the max size 
   * allowed for packing 
   */ 
  address = Top-Rhs+2;
  C2F(varpak)(&address,Ipack,&used,&size,&ierr); 
  switch (ierr ) { 
  case 1: Scierror(999,_("%s: work space (stacksize) is too small.\n"),fname);
    return 0;
  case 2: Scierror(999,_("%s: Unknown type or not yet implemented.\n"),fname);
    return 0; 
  }
  C2F(scipvmbcast)(cstk(l1),&m1,Ipack,&used,(double *)header,istk(l3),istk(l4));
  LhsVar(1)= Rhs+4;
  pvm_error_check(fname,*istk(l4),fname_len);
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
