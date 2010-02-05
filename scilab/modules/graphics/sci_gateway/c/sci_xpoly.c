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
/* file: sci_xpoly.c                                                      */
/* desc : interface for xpoly routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_xpoly( char * fname, unsigned long fname_len )
{
  int m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2;

  long hdl;/* NG */
  int mark;/* NG */
  sciPointObj * pobj    = NULL ;
  sciPointObj * psubwin = NULL ;
  sciPointObj * pFigure = NULL;

  CheckRhs(2,4);
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  mn2 = m2 * n2;

  if (Rhs >= 3) {
    GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
    if ( strcmp(cstk(l3),"lines") == 0) {
      strcpy(C2F(cha1).buf,"xlines"); 
      mark=1; /* NG */
    } else if (strcmp(cstk(l3),"marks") == 0) {
      strcpy(C2F(cha1).buf,"xmarks"); 
      mark=0; /* NG */
    } else {
      Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,3, "lines","marks");
      return 0;
    } 
  }
  else {
    strcpy(C2F(cha1).buf,"xlines");
    mark=1; /* NG */
  }

  if (Rhs >= 4) { GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4); close = (int)  *stk(l4);} 
  /* NG beg */

  
  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();
  endGraphicDataWriting();

  
  Objpoly (stk(l1),stk(l2),mn2,close,mark,&hdl);
  pobj = sciGetCurrentObj(); /* the polyline newly created */
  startFigureDataWriting(pFigure);
  if(mark == 0){ 
    /* marks are enabled but markstyle & foreground 
    is determined by parents' markstyle & foreground */
    sciSetIsMark(pobj, TRUE);
    sciSetIsLine(pobj,FALSE);
    sciSetMarkStyle (pobj,sciGetMarkStyle(psubwin));
    sciSetForeground (pobj, sciGetForeground (psubwin));
  }
  else{
    sciSetIsMark(pobj, FALSE);
    sciSetIsLine(pobj, TRUE);
    sciSetLineStyle(pobj, sciGetLineStyle (psubwin));
    sciSetForeground (pobj, sciGetForeground (psubwin));
  }
  endFigureDataWriting(pFigure);

  startFigureDataReading(pFigure);
  sciDrawObjIfRequired(pobj);
  endFigureDataReading(pFigure);

  /* NG end */
  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
}

/*--------------------------------------------------------------------------*/
