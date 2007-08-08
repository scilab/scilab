/*------------------------------------------------------------------------*/
/* file: sci_xdel.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xdel.h"
#include "stack-c.h"
#include "Xcall1.h"
#include "DestroyObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_xdel(char *fname,unsigned long fname_len)
{ 
  integer verb=0,na,v,m1,n1,l1,win;
  double dv;
  CheckRhs(-1,1);
  if (Rhs >= 1) {
    int i;
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    for ( i=0; i < m1*n1 ; i++ ) 
    {
      win = (integer) *stk(l1+i);
      sciDeleteWindow( win ) ;
    }
  } else {
    C2F(dr)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
    sciDeleteWindow( win ) ;
  }
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
