/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_xpoly( char * fname, unsigned long fname_len )
{
  int m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2;

  long hdl;/* NG */
  int mark;/* NG */
  int markMode;
  int lineMode;
  int foreground;
  int iTmp = 0;
  int* piTmp = &iTmp;
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

  /* Deactivated for now (synchronization) */
#if 0
  startGraphicDataWriting();
#endif

  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();

  /* Deactivated for now (synchronization) */
#if 0
  endGraphicDataWriting();
#endif

  Objpoly (stk(l1),stk(l2),mn2,close,mark,&hdl);

  pobj = sciGetCurrentObj(); /* the polyline newly created */

  /* Deactivated for now (synchronization) */
#if 0
  startFigureDataWriting(pFigure);
#endif

  /*
   * The contour properties set calls below were
   * already present and have been updated for the MVC.
   */
  if(mark == 0){
    /* marks are enabled but markstyle & foreground
    is determined by parents' markstyle & foreground */

    markMode = 1;
    lineMode = 0;

    getGraphicObjectProperty(psubwin->UID, __GO_MARK_STYLE__, jni_int, &piTmp);
    sciInitMarkStyle(pobj, iTmp);
  }
  else{
    markMode = 0;
    lineMode = 1;

    getGraphicObjectProperty(psubwin->UID, __GO_LINE_STYLE__, jni_int, &piTmp);
    sciInitLineStyle(pobj, iTmp);
  }

  getGraphicObjectProperty(psubwin->UID, __GO_LINE_COLOR__, jni_int, &piTmp);
  foreground = iTmp;

  setGraphicObjectProperty(pobj->UID, __GO_LINE_COLOR__, &foreground, jni_int, 1);

  setGraphicObjectProperty(pobj->UID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
  setGraphicObjectProperty(pobj->UID, __GO_LINE_MODE__, &lineMode, jni_bool, 1);


 /*
  * Deactivated for now as it performs drawing operations
  * To be implemented
  */
#if 0
  endFigureDataWriting(pFigure);

  startFigureDataReading(pFigure);
  sciDrawObjIfRequired(pobj);
  endFigureDataReading(pFigure);
#endif

  /* NG end */
  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
}

/*--------------------------------------------------------------------------*/
