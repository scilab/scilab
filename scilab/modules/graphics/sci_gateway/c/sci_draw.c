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
/* file: sci_draw.h                                                       */
/* desc : interface for draw routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_draw.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "HandleManagement.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_draw( char * fname, unsigned long fname_len )
{ 
  unsigned long hdl;
  sciPointObj *pobj, *psubwin, *tmpsubwin;
  integer m,n,l,lw;

  CheckRhs(0,1) ;
  CheckLhs(0,1) ;

  if (Rhs == 0)
  {
    pobj = sciGetCurrentObj() ;
  }
  else
  {
    GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &m, &n, &l ); 
    if (m!=1||n!=1)
    {
      lw = 1 + Top - Rhs ;
      C2F(overload)(&lw,"draw",4);
      return 0;
    }
    hdl = (unsigned long)*hstk(l);            /* Puts the value of the Handle to hdl */ 
    pobj = sciGetPointerFromHandle(hdl);   
  }
  if (pobj != NULL )
  {  
    tmpsubwin = sciGetCurrentSubWin(); 
    psubwin = sciGetParentSubwin(pobj);
    if ( psubwin != NULL )
    {
      sciSetSelectedSubWin(psubwin); 
      set_scale ("tttftt", pSUBWIN_FEATURE (psubwin)->WRect, 
        pSUBWIN_FEATURE (psubwin)->FRect,
        NULL, pSUBWIN_FEATURE (psubwin)->logflags, 
        pSUBWIN_FEATURE (psubwin)->ARect); 

      sciDrawObj(pobj); 
      sciSetSelectedSubWin(tmpsubwin);
    }
    else
    {
      Scierror(999,_("%s: Object has no parent.\n"),fname);
      return 0;
    }
  }

  LhsVar(1) = 0;
  return 0;
}
/*--------------------------------------------------------------------------*/
