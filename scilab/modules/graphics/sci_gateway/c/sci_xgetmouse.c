/*------------------------------------------------------------------------*/
/* file: sci_xgetmouse.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* Java version: Vincent COUVERT (2007)                                   */
/* desc : interface for sci_xgetmouse routine                             */
/*------------------------------------------------------------------------*/

#include "sci_xgetmouse.h"
#include "stack-c.h"
#include "GetProperty.h" /* sciGetNum */
#include "CurrentObjectsManagement.h" /* sciGetCurrentFigure */
#include "CallJxgetmouse.h"

/*--------------------------------------------------------------------------*/
int sci_xgetmouse( char *fname,unsigned long fname_len )
{
  integer  m1=1,n1=3,l1,l2;
  int mouseButtonNumber = 0;
  integer sel[2],m,n;
  double xCoordinate = 0.0;
  double yCoordinate = 0.0;

  CheckRhs(0,2);
  CheckLhs(1,2);

  if (Rhs<=0)
    {
      // Call Java xgetmouse
      CallJxgetmouse();
    }
  else
    {
      if (GetType(1)==sci_boolean)
        {
          GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l1);
          CheckDims(1,m*n,1,2,1);
          sel[0]=*istk(l1);
          sel[1]=*istk(l1+1);

          // Call Java xgetmouse
          CallJxgetmouseWithOptions(sel[0], sel[1]);
        }
      else
        {
          // Call Java xgetmouse
         CallJxgetmouse();
        }
/*       if (Rhs==2) */
/*         { */
/*           GetRhsVar(2,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l1); */
/*           CheckDims(2,m*n,1,2,1); */
/*           sel[0]=*istk(l1); */
/*           sel[1]=*istk(l1+1); */
/*         } */
    }
  
  // Get return values
  mouseButtonNumber = getJxgetmouseMouseButtonNumber();
  xCoordinate = getJxgetmouseXCoordinate();
  yCoordinate = getJxgetmouseYCoordinate();

  switch (Lhs) {
  case 1: 
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    if (mouseButtonNumber==-100)
    {
      *stk(l1) = -1;
      *stk(l1+1) = -1;
      *stk(l1+2) = (double) mouseButtonNumber;
    }
    else
    {
      *stk(l1) = xCoordinate;
      *stk(l1+1) = yCoordinate;
      *stk(l1+2) = (double) mouseButtonNumber;
    }
    LhsVar(1) = Rhs+1;
    return 0;
  case 2:
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    if (mouseButtonNumber==-100)
    {
      *stk(l1) = -1;
      *stk(l1+1) = -1;
      *stk(l1+2) = (double) mouseButtonNumber;
    }
    else
    {
      *stk(l1) = xCoordinate;
      *stk(l1+1) = yCoordinate;
      *stk(l1+2) = (double) mouseButtonNumber;
    }
    LhsVar(1) = Rhs+1;

    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&m1,&l2);
    *stk(l2) = sciGetNum(sciGetCurrentFigure()); /* this is the window number */
    LhsVar(2) = Rhs+2;
    return 0;
  }
  return -1 ;
} 

/*--------------------------------------------------------------------------*/
