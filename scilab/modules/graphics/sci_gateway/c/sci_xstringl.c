/*------------------------------------------------------------------------*/
/* file: sci_xstringl.c                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstringl routine                                  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xstringl.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "MALLOC.h"
#include "DrawObjects.h"
#include "Axes.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_xstringl( char *fname, unsigned long fname_len )
{
  integer m4,n4,l4,m1,n1,l1,m2,n2,l2,m3,n3;
  integer m5,n5,l5;
  int one = 1;
  int four = 4;
  char **Str;

  CheckRhs(3,5);
  CheckLhs(0,1);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); CheckScalar(2,m2,n2);
  GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Str);

  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4) { GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4);}
  if (Rhs >= 5) { GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&m5,&n5,&l5); CheckScalar(5,m5,n5);}


  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&four,&l4);
  *stk(l4) = 0.0; 
  *stk(l4+1) = 0.0;
  *stk(l4+2) = 0.0;
  *stk(l4+3) = 0.0;

  freeArrayOfString(Str,m3*n3);

  LhsVar(1)=Rhs+1;
  return 0;
}

/*--------------------------------------------------------------------------*/
