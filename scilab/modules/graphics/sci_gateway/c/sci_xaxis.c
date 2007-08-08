/*------------------------------------------------------------------------*/
/* file: sci_xaxis.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xaxis routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xaxis.h"
#include "Xcall1.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xaxis(char *fname,unsigned long fname_len)
{
  integer v;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  double dv;

  CheckRhs(2,4);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2); CheckLength(1,m2*n2,2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckLength(1,m3*n3,3);
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckLength(1,m4*n4,2);

  SciWin();
  C2F(dr1)("xaxis","v",&v,istk(l2),&v,&v,&v,&v,stk(l1),stk(l3),stk(l4),&dv,6L,2L);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
