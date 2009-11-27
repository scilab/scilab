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

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "MALLOC.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "GraphicSynchronizerInterface.h"
#include "DrawObjects.h"
#include "Axes.h" /* propertyNameToLegendPlace */
#include "SetProperty.h" /* sciSetLegendLocation */
#include "HandleManagement.h"

#define DEF_LEGEND_LOCATION "in_upper_right"

/*--------------------------------------------------------------------------*/
int sci_Legend( char * fname, unsigned long fname_len )
{
  int numrow,numcol,l1,l2,n,m2,n2;
  long handelsvalue = 0 ;
  int outindex,i;
  sciPointObj *pobj;
  long long *tabofhandles;
  sciPointObj * psubwin = NULL;
  sciPointObj * pFigure = NULL;
  sciLegendPlace location;
  sciEntityType type;
  char **Str = NULL;
	sciPointObj * legend;

  CheckRhs(2,3);
  CheckLhs(0,1);

  
  GetMatrixdims(1,&numrow,&numcol);
  n=numrow*numcol;
  if (numrow==0 || numcol==0) 
  {
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numrow,&numcol,&l1);
    LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
    return 0;
  }
  GetMatrixdims(2,&m2,&n2);
  if (m2*n2 != n) 
  {
    Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: Incompatible length.\n"),fname,1,2);
    return 0;
  }


  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&l1); 
  GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str);
  if (Rhs==3) 
  {
    GetRhsVar(3,STRING_DATATYPE,&m2,&n2,&l2);
    location = propertyNameToLegendPlace(cstk(l2));
    if (location == SCI_LEGEND_POSITION_UNSPECIFIED) 
	{
      Scierror(999,_("%s: Wrong value for input argument #%d: Incorrect value.\n"),fname,3);
      return 0;
    }
  }
  else 
  {
    location = propertyNameToLegendPlace(DEF_LEGEND_LOCATION);
  }

  tabofhandles = (long long *)MALLOC(n*sizeof(long long));
  if (tabofhandles == NULL) {
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
    handelsvalue = (unsigned long) (hstk(l1))[n-1-i];

    if (psubwin!=sciGetParentSubwin( sciGetPointerFromHandle(handelsvalue) )) 
	{
      Scierror(999,_("%s: Objects must have the same axes.\n"),fname);
      return 0;
    }

    pobj = sciGetPointerFromHandle(handelsvalue);
    if (pobj == NULL) 
	{
      freeArrayOfString(Str,n);
      FREE(tabofhandles);
      Scierror(999,_("%s: The handle is no more valid.\n"),fname);
      return 0;
    }
    type=sciGetEntityType(pobj);
    if (type != SCI_POLYLINE) 
	{
      freeArrayOfString(Str,n);
      FREE(tabofhandles);
      Scierror(999,_("%s: The %d th handle is not a polyline handle.\n"),fname,i+1);
      return 0;
    }
    tabofhandles[i]=handelsvalue;
    
  }

  /* Create the legend */
  legend = ConstructLegend (psubwin, Str, tabofhandles, n);
  sciSetLegendLocation(legend, location);

  /* Draw it */
  sciSetCurrentObj(legend);
  startFigureDataReading(pFigure);
  sciDrawObjIfRequired(legend);
  endFigureDataReading(pFigure);

  freeArrayOfString(Str,n);
  FREE(tabofhandles);

  /* Return the handle of the newly create dlegend */
  numrow = 1;
  numcol = 1;
  CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
  hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = Rhs+1;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
