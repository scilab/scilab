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
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_xfarcs( char * fname, unsigned long fname_len )
{
  int m1,n1,l1,m2,n2,l2;
  long  hdl;
  int i;
  double angle1;
  double angle2;
  sciPointObj * pFigure = NULL;

  CheckRhs(1,2);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  if (m1 != 6) {
    Scierror(999,_("%s: Wrong size for first input argument: %s expected.\n"),fname,"(6,n)");
    return 0;
  }

  if (Rhs == 2) {
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    CheckVector(2,m2,n2);
    if ( n1 != m2*n2) {
      Scierror(999,_("%s: Input arguments have incompatible size.\n"),fname);
      return 0;
    }
  }
  else 
  {
    m2=1,n2=n1; CreateVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    for (i = 0; i < n2; ++i) { *istk(l2 + i) = i+1 ; }
  }

  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  endGraphicDataWriting();
  for (i = 0; i < n1; ++i)
  { 
    angle1 = DEG2RAD(*stk(l1+(6*i)+4) / 64.0);
    angle2 = DEG2RAD(*stk(l1+(6*i)+5) / 64.0);
    Objarc (&angle1,&angle2,stk(l1+(6*i)),stk(l1+(6*i)+1),
      stk(l1+(6*i)+2),stk(l1+(6*i)+3),istk(l2+i),istk(l2+i),TRUE,FALSE,&hdl); 
  }
  /** construct Compound and make it current object **/
  startFigureDataWriting(pFigure);
  sciSetCurrentObj(ConstructCompoundSeq (n1));
  endFigureDataWriting(pFigure);

  LhsVar(1)=0;
  return 0;

}
/*--------------------------------------------------------------------------*/
