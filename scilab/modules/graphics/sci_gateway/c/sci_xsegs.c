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
/* file: sci_xsegs.c                                                      */
/* desc : interface for xsegs routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xsegs.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_xsegs(char *fname,unsigned long fname_len)
{
  integer dstyle = -1, *style,flag;
  integer mn2;
  integer m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3; 
  double arsize = 0.0 ;
  sciPointObj * psubwin = NULL ;


  CheckRhs(2,3);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  if (m2*n2 == 0) { LhsVar(1)=0; return 0;} 

  if (Rhs == 3)
  {
    GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3); 
	CheckVector(3,m3,n3);
    if (m3 * n3 == 1) dstyle = *istk(l3 );
    if (m3 * n3 != 1 && m2 * n2 / 2 != m3 * n3) {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d expected.\n"),fname, 3, m2 * n2 / 2);
      return 0;
    }
  }
  mn2 = m2 * n2; 

  psubwin = sciGetCurrentSubWin();

  if (Rhs == 3 && m3 * n3 != 1)
  {
    style = istk(l3);
    flag = 1 ;
  }
  else if (Rhs == 3 && m3 * n3 == 1) {
    style = istk(l3);
    flag = 0 ;
  }
  else
  { /* Rhs < 3 => no color specified, use current color (taken from axes parent) */
    int col = sciGetForegroundToDisplay(psubwin);
    style = &col;
    flag= 0 ;
  }

  Objsegs (style,flag,mn2,stk(l1),stk(l2),arsize);

  sciDrawObjIfRequired(sciGetCurrentObj ());
    

  LhsVar(1)=0;
  return 0;
} 
/*--------------------------------------------------------------------------*/
