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
/* file: sci_xpolys.c                                                     */
/* desc : interface for xpolys routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"


/*--------------------------------------------------------------------------*/
int sci_xpolys(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3 ;
  int i;
  long hdl;
  sciPointObj * pFigure = NULL;

  CheckRhs(2,3);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if ( m1 * n1 == 0 || m2 * n2 == 0 )
  {
    /* dimension 0, 0 polyline to draw */
    LhsVar(1)=0 ;
		C2F(putlhsvar)();
    return 0 ;
  }

  pFigure = sciGetCurrentFigure();

  startFigureDataWriting(pFigure);
  if (Rhs == 3) 
  {
    GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m3,&n3,&l3); CheckVector (3,m3,n3);
    CheckDimProp(1,3,m3 * n3 < n1);
    /* Construct the polylines */
    for (i = 0; i < n1; ++i)
    {
      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m2)),m1,0,*istk(l3+i),&hdl);
    }
  }
  else
  {
    for (i = 0; i < n1; ++i)
    {
      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m2)),m1,0,1,&hdl);
    }
  } 


  /** construct Compound and make it current object**/
  sciSetCurrentObj (ConstructCompoundSeq (n1));

  endFigureDataWriting(pFigure);

  sciDrawObjIfRequired(sciGetCurrentObj ());

  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
}

/*--------------------------------------------------------------------------*/
