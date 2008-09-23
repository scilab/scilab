/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xarcs.h                                                       */
/* desc : interface for xarcs routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xarcs.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"
#include "DrawingBridge.h"

/*--------------------------------------------------------------------------*/
int sci_xarcs(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2;

  /* NG beg */
  long  hdl;
  int i;
  double angle1;
  double angle2;
  sciPointObj * pFigure = NULL;
  /* NG end */
  CheckRhs(1,2);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  if ( strcmp(fname,"xarcs") == 0) {
    if (m1 != 6) {
      Scierror(999,_("%s: Wrong size for input argument #%d: %s expected.\n"),fname,1,"(6,n)");
      return 0;
    }
  } else {
    if (m1 != 4) {
      Scierror(999,_("%s: Wrong size for input argument #%d: %s expected.\n"),fname,1,"(4,n)");
      return 0;
    }
  }

  pFigure = sciGetCurrentFigure();

  if (Rhs == 2) 
  {
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    CheckVector(2,m2,n2);
    if (m2 * n2 != n1) {
      Scierror(999,_("%s: Wrong size for arguments #%d and #%d.\n"),fname, 1, 2);
      return 0;
    }
  }
  else 
  {
    int i2;
    m2=1,n2=n1; CreateVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
    startFigureDataReading(pFigure);
    for (i2 = 0; i2 < n2; ++i2)
    { 
      *istk(l2 + i2) = sciGetForeground(sciGetCurrentSubWin() );
    }
    endFigureDataReading(pFigure);
  }

  /* NG beg */
  startFigureDataWriting(sciGetCurrentFigure());
  for (i = 0; i < n1; ++i)
  { 
    angle1 = DEG2RAD(*stk(l1+(6*i)+4) / 64.0);
    angle2 = DEG2RAD(*stk(l1+(6*i)+5) / 64.0);
    Objarc (&angle1,&angle2,stk(l1+(6*i)),stk(l1+(6*i)+1),
      stk(l1+(6*i)+2),stk(l1+(6*i)+3),istk(l2+i),NULL,FALSE,TRUE,&hdl); 
  }
  /* construct Compound and make it current object */
  sciSetCurrentObj(ConstructCompoundSeq(n1));
  endFigureDataWriting(pFigure);

  /* Draw the result */
  startFigureDataReading(pFigure);
  sciDrawObj(sciGetCurrentObj());
  endFigureDataReading(pFigure);

  /* NG end */
  LhsVar(1)=0;
  return 0;
} 
/*--------------------------------------------------------------------------*/
