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
/* file: sci_xfpoly.c                                                     */
/* desc : interface for xfpoly routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xfpoly.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "SetProperty.h"
#include "ObjectStructure.h"
#include "DrawObjects.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"

/*--------------------------------------------------------------------------*/
int sci_xfpoly(char *fname,unsigned long fname_len)
{
  int close=0,m1,n1,l1,m2,n2 ,l2,m3,n3,l3,mn1 ;

  long hdl; /* NG */
  sciPointObj * psubwin = NULL ;

  CheckRhs(2,3);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if (Rhs == 3) {
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
    CheckScalar(3,m3,n3);
    close = (int)  *stk(l3);
  } 
  mn1 = m1 * n1;


  psubwin = sciGetCurrentSubWin();

  if(close == 0)
  {
    close = sciGetForeground(sciGetCurrentSubWin());
  }

  startFigureDataWriting(sciGetParentFigure(psubwin));
  Objfpoly (stk(l1),stk(l2),mn1,&close,&hdl,0);
  endFigureDataWriting(sciGetParentFigure(psubwin));

  sciDrawObjIfRequired(sciGetCurrentObj ());

  LhsVar(1)=0;
  return 0;

}

/*--------------------------------------------------------------------------*/
