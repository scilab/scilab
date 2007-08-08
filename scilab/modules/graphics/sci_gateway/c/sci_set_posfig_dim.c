/*------------------------------------------------------------------------*/
/* file: sci_set_posfig_dim.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for set_posfig_dim routine                            */
/*------------------------------------------------------------------------*/

#include "sci_set_posfig_dim.h"
#include "periPos.h"

/*-----------------------------------------------------------------------------------*/
int sci_set_posfig_dim( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
  setposfig(istk(l1),istk(l2));
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
