/*------------------------------------------------------------------------*/
/* file: sci_xgetmouse.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xgetmouse routine                             */
/*------------------------------------------------------------------------*/

#include "sci_xgetmouse.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "Xcall1.h"

/*-----------------------------------------------------------------------------------*/
int sci_xgetmouse( char *fname,unsigned long fname_len )
{
  integer  m1=1,n1=3,l1,l2,button,v;
  integer iflag;
  integer sel[2],m,n,v2;
  double x,y,dv;

  CheckRhs(0,2);
  CheckLhs(1,2);

  if (Rhs<=0)
  {
    iflag=0;sel[0]=1;sel[1]=0;
  }
  else {
    if (GetType(1)==4) {
      iflag=0;
      GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l1);
      CheckDims(1,m*n,1,2,1);
      sel[0]=*istk(l1);sel[1]=*istk(l1+1);}
    else {
      iflag=1;
      sel[0]=1;sel[1]=0;
    }
    if (Rhs==2) {
      if (iflag==1) {
        GetRhsVar(2,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l1);
        CheckDims(2,m*n,1,2,1);
        sel[0]=*istk(l1);sel[1]=*istk(l1+1);}
    }
  }

  SciWin();

  switch (Lhs) {
  case 1: 
    v2=0;
    C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,sel,&v2,&x,&y,&dv,&dv,10L,3L); 
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    if (button==-100)
    {
      *stk(l1) = -1;
      *stk(l1+1) = -1;
      *stk(l1+2) = (double) button;
    }
    else
    {
      *stk(l1) = x;  *stk(l1+1) = y;  *stk(l1+2) = (double) button;
    }
    LhsVar(1) = Rhs+1;
    return 0;
  case 2:
    v2=2;
    C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,sel,&v2,&x,&y,&dv,&dv,10L,3L);
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    if (button==-100)
    {
      *stk(l1) = -1;
      *stk(l1+1) = -1;
      *stk(l1+2) = (double) button;
    }
    else
    {
      *stk(l1) = x;  *stk(l1+1) = y;  *stk(l1+2) = (double) button;
    }
    LhsVar(1) = Rhs+1;

    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&m1,&l2);
    *stk(l2) = iflag; /* this is the window number */
    LhsVar(2) = Rhs+2;
    return 0;
  }
  return -1 ;
} 

/*-----------------------------------------------------------------------------------*/
