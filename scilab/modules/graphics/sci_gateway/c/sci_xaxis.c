/*------------------------------------------------------------------------*/
/* file: sci_xaxis.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xaxis routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xaxis.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xaxis(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  CheckRhs(2,4);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2); CheckLength(1,m2*n2,2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckLength(1,m3*n3,3);
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckLength(1,m4*n4,2);

  /* TODO: transalte in new graphic */
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
