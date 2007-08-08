/*------------------------------------------------------------------------*/
/* file: sci_xclea.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclea routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xclea.h"
#include "stack-c.h"
#include "Xcall1.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xclea( char *fname,unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  integer v;

  SciWin();
  CheckRhs(1,4);
  switch ( Rhs ) 
  {
  case 1 :
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    CheckLength(1,m1*n1,4);
    C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),fname_len,2L);      
    break;
  case 4 :
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); CheckScalar(2,m2,n2);
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckScalar(3,m3,n3);
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4);
    C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,stk(l1),stk(l2),stk(l3),stk(l4),fname_len,2L);
    break;
  default :
    Scierror(999,"%s: wrong number of rhs argumens (%d), rhs must be 1 or 4\r\n",fname,Rhs);
  }
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
