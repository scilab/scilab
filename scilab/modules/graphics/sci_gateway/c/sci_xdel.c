/*------------------------------------------------------------------------*/
/* file: sci_xdel.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xdel.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_xdel(char *fname,unsigned long fname_len)
{ 
  integer m1,n1,l1;
  CheckRhs(-1,1);
  if (Rhs >= 1) {
    int i;
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    for ( i=0; i < m1*n1 ; i++ ) 
    {
      int win = (int) *stk(l1+i);
      sciDeleteWindow( win ) ;
    }
  } else {
    sciDeleteWindow( sciGetNum(sciGetCurrentFigure()) ) ;
  }
  LhsVar(1)=0;
  return 0;
} 

/*--------------------------------------------------------------------------*/
