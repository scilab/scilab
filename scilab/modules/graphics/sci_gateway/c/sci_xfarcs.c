/*------------------------------------------------------------------------*/
/* file: sci_xfarcs.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xfarcs routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xfarcs.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "sciCall.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_xfarcs( char * fname, unsigned long fname_len )
{
  int m1,n1,l1,m2,n2,l2;
  long  hdl;
  int i,a1,a2;

  SciWin();
  CheckRhs(1,2);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  if (m1 != 6) {
    Scierror(999,"%s: arcs has a wrong size (6,n) expected \r\n",fname);
    return 0;
  }

  if (Rhs == 2) {
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    CheckVector(2,m2,n2);
    if ( n1 != m2*n2) {
      Scierror(999,"%s: arguments have incompatible size\r\n",fname);
      return 0;
    }
  }
  else 
  {
    m2=1,n2=n1; CreateVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    for (i = 0; i < n2; ++i) { *istk(l2 + i) = i+1 ; }
  }

  for (i = 0; i < n1; ++i)
  { 
    a1 = (int)(*stk(l1+(6*i)+4));
    a2 = (int)(*stk(l1+(6*i)+5));
    Objarc (&a1,&a2,stk(l1+(6*i)),stk(l1+(6*i)+1),
      stk(l1+(6*i)+2),stk(l1+(6*i)+3),istk(l2+i),istk(l2+i),TRUE,FALSE,&hdl); 
  }
  /** construct Compound and make it current object **/
  sciSetCurrentObj (ConstructCompoundSeq (n1));

  LhsVar(1)=0;
  return 0;

}
/*-----------------------------------------------------------------------------------*/
