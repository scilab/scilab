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
/* file: sci_xarc.h                                                       */
/* desc : interface for xarc routine                                      */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "stack-c.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "DrawingBridge.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xarc(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,l2,l3,l4,l5,l6;
  long hdl;
  int* tmp;
  int curcolor = 0;
  int *piCurColor = &curcolor;
  sciPointObj * psubwin = NULL;
  sciPointObj * pFigure = NULL;
  double angle1 = 0.0;
  double angle2 = 0.0;


  CheckRhs(6,6);
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);CheckScalar(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);CheckScalar(2,m1,n1);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);CheckScalar(3,m1,n1);
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);CheckScalar(4,m1,n1);
  GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l5);CheckScalar(5,m1,n1);
  GetRhsVar(6,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l6);CheckScalar(6,m1,n1);

  angle1 = DEG2RAD(*istk(l5) / 64.0); /* convert to radian */
  angle2 = DEG2RAD(*istk(l6) / 64.0);

  /* Deactivated (synchronization) */
#if 0
  startGraphicDataWriting();
#endif

  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();

  /* Deactivated (synchronization) */
#if 0
  endGraphicDataWriting();
#endif

  /* Deactivated (synchronization) */
#if 0
  startFigureDataReading(pFigure);
  curcolor = sciGetForeground(psubwin); /* current color equivalent for new graphics mode */
  endFigureDataReading(pFigure);
#endif

  getGraphicObjectProperty(psubwin->UID, __GO_LINE_COLOR__, jni_int, &piCurColor);

  /* Deactivated (synchronization) */
#if 0
  startFigureDataWriting(pFigure);
#endif
  if ( strcmp(fname,"xarc") == 0 )
  {
    Objarc (&angle1,&angle2,stk(l1),stk(l2),stk(l3),stk(l4),&curcolor,NULL,FALSE,TRUE,&hdl);
  }
  else /* xfarc case */
  {
    Objarc (&angle1,&angle2,stk(l1),stk(l2),stk(l3),stk(l4),NULL,&curcolor,TRUE,FALSE,&hdl);
  }
  /* Deactivated (synchronization) */
#if 0
  endFigureDataWriting(pFigure);
#endif

  /*
   * Deactivated, sinces it performs drawing
   * To be implemented
   */
#if 0
  sciDrawObj(sciGetCurrentObj());
#endif

  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
