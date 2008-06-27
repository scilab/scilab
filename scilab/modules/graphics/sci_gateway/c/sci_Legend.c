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
/* file: sci_glue.h                                                       */
/* desc : interface for glue routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_Legend.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "GraphicSynchronizerInterface.h"
#include "DrawObjects.h"

/*--------------------------------------------------------------------------*/
int sci_Legend( char * fname, unsigned long fname_len )
{
  integer numrow,numcol,l1,n,m2,n2;
  long handelsvalue = NULL ;
  int outindex,i;
  int *pstyle;
  sciPointObj *pobj;
  sciPointObj **pptabofpointobj;
  sciPointObj * psubwin = NULL;
  sciPointObj * pFigure = NULL;

  sciEntityType type;
  char **Str = NULL;

  CheckRhs(2,2);
  CheckLhs(0,1);

  
  GetMatrixdims(1,&numrow,&numcol);
  n=numrow*numcol;
  if (numrow==0 || numcol==0) {
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numrow,&numcol,&l1);
    LhsVar(1) = Rhs+1;
    return 0;
  }
  GetMatrixdims(2,&m2,&n2);
  if (m2*n2 != n) {
    Scierror(999,_("%s: Input arguments %d and %d have incompatible dimensions.\n"),fname,1,2);
    return 0;
  }


  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&l1); 
  GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str);
  
  pptabofpointobj = (sciPointObj **)MALLOC(n*sizeof(sciPointObj *));
  if (pptabofpointobj == NULL) {
    freeArrayOfString(Str,n);
    Scierror(999,_("%s: No more memory.\n"),fname);
    return 0;
  }
    
  startGraphicDataWriting();
  pFigure = sciGetCurrentFigure();
  psubwin = sciGetCurrentSubWin();
  endGraphicDataWriting();


  for (i = 0; i < n;i++)
  {
    handelsvalue = (unsigned long) (hstk(l1))[i];
    if (psubwin!=sciGetParentSubwin(handelsvalue)) {
      Scierror(999,_("%s: Objects must have the same axes.\n"),fname);
      return 0;
    }

    pobj = sciGetPointerFromHandle(handelsvalue);
    if (pobj == NULL) {
      freeArrayOfString(Str,n);
      FREE(pptabofpointobj);
      Scierror(999,_("%s: No more memory.\n"),fname);
      return 0;
    }
    type=sciGetEntityType(pobj);
    if (type != SCI_POLYLINE) {
      freeArrayOfString(Str,n);
      FREE(pptabofpointobj);
      Scierror(999,_("%s: The %d th handle is not a polyline handle.\n"),fname,i+1);
      return 0;
    }
    pptabofpointobj[i]=pobj;
    
  }
  sciSetCurrentObj ((sciPointObj *)ConstructLegend (psubwin, Str, 
						    0, n, pstyle , pptabofpointobj));
  startFigureDataReading(pFigure);
  sciDrawObjIfRequired(sciGetCurrentObj ());
  endFigureDataReading(pFigure);

  freeArrayOfString(Str,n);
  FREE(pptabofpointobj);
  numrow = 1;
  numcol = 1;
  CreateVar(3,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
  hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = 3;
  return 0;
}
/*--------------------------------------------------------------------------*/
