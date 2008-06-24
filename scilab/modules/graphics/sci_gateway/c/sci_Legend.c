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
#include "../../elementary_functions/includes/elementary_functions.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_Legend( char * fname, unsigned long fname_len )
{
  integer numrow,numcol,l1,l2,lind,n,cx1=1,ret,m2,n2,nblegends ;
  unsigned long hdl = 0, parenthdl = 0 ;
  long handelsvalue = NULL ;
  int outindex,i;
  int *pstyle;
  sciPointObj *pobj;
  sciPointObj * pparentsubwin;
  sciPointObj **pptabofpointobj;
  sciEntityType type;
  char **Str = NULL;

  CheckRhs(2,2);
  CheckLhs(0,1);


  /*  set or create a graphic window */
  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&l1); 
  GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str);
  n=numrow*numcol;
  if (n==0) {
    Scierror(999,_("%s: Input argument #%d must not be empty.\n"),fname,1);
    return 0;
  }

  if (m2*n2 != n) {
    freeArrayOfString(Str,n);
    Scierror(999,_("%s: Input arguments %d and %d have incompatible dimensions.\n"),fname,1,2);
    return 0;
  }
  numrow = 1;
  numcol = 1;
  CreateVar(3,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
  hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = 3;

  strcpy(C2F(cha1).buf,Str[0]);
  for ( i= 1 ; i <n ; i++) 
    {
      strcat(C2F(cha1).buf,"@"); 
      strcat(C2F(cha1).buf,Str[i]);
    }
  freeArrayOfString(Str,n);

 
  /* we must change the pobj to the Compound type */
  pptabofpointobj = (sciPointObj **)MALLOC(n*sizeof(sciPointObj *));
  if (pptabofpointobj == NULL) {
      Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
      return 0;
    }
    
  for (i = 0; i < n;i++)
  {
    handelsvalue = (unsigned long) (hstk(l1))[i];
    if (i==0)
      pparentsubwin = sciGetParentSubwin(handelsvalue);
    else
      if (pparentsubwin!=sciGetParentSubwin(handelsvalue)) {
	Scierror(999,_("%s: Objects must have the same axes.\n"),fname);
	return 0;
      }

    pobj = sciGetPointerFromHandle(handelsvalue);
    if (pobj == NULL) {
      Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
      return 0;
    }
    type=sciGetEntityType(pobj);
    if (type != SCI_POLYLINE) {
      Scierror(999,_("%s: The %d th handle is not a polyline handle.\n"),fname,i+1);
      return 0;
    }
    pptabofpointobj[i]=pobj;
    
  }
  sciSetCurrentObj ((sciPointObj *)ConstructLegend (pparentsubwin, C2F(cha1).buf, 
						    0, n, pstyle , pptabofpointobj));
  FREE(pptabofpointobj);
  numrow = 1;
  numcol = 1;
  CreateVar(3,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
  hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = 3;
  return 0;
}
/*--------------------------------------------------------------------------*/
