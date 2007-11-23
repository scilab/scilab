/*--------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_send, fin = 22
 ******************************************/
int intspvm_send _PARAMS((char *fname,unsigned long fname_len))
{
  int *Ipack,*header,address;
  int m1,n1,l1,mn1,m3,n3,l3,un=1,l4,size,l5,used,ierr;
  CheckRhs(3,3);
  CheckLhs(1,1);
  /*  checking variable tids */
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  CheckRow(1,m1,n1);
  mn1=m1*n1;
  /*  the data at position 2 will be packed by varpack */
  header = GetData(2);
  /*  checking variable tag */
  GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3);
  CheckScalar(3,m3,n3);
  /* cross variable size checking */
  CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&un,&un,&l4);/* named: res */
  CreateWork(5,&size,&l5); 
  Ipack= (int *) stk(l5);
  /* Build a pack vector for buff: size is the max size 
   * allowed for packing 
   */ 
  address = Top-Rhs+2;
  C2F(varpak)(&address,Ipack,&used,&size,&ierr); 
  switch (ierr ) { 
  case 1: Scierror(999,_("%s: work space (stacksize) is too small d\n"),fname);
    return 0;
  case 2: Scierror(999,_("%s: Unknow type or not yet implemented\n"),fname);
    return 0; 
  }

  C2F(scipvmsend)(istk(l1),&n1,Ipack,&used,(double *)header,istk(l3),istk(l4));
  LhsVar(1)= Rhs+4;

  pvm_error_check(fname,*istk(l4),fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/ 
