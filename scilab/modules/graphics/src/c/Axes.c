/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*------------------------------------------------------------------------
 *    Graphic library
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * Axis drawing for 2d plots 
 *--------------------------------------------------------------------------*/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "math_graphics.h"
#include "Axes.h"
#include "DestroyObjects.h"
#include "DrawObjects.h"
#include "InitObjects.h"
#include "SetProperty.h"
#include "axesScale.h"
#include "CurrentObjectsManagement.h"


#include "MALLOC.h" /* MALLOC */

#include "GetProperty.h"

/*--------------------------------------------------------------------------------*/
/* clear a subwindow from all of its children */
void clearSubWin( sciPointObj * pSubWin )
{
  sciSons * curSon = sciGetSons (pSubWin);
  
  while ( curSon != NULL && curSon->pointobj != NULL )
  {
    if ( curSon->pointobj->entitytype != SCI_LABEL )
    {
      destroyGraphicHierarchy (curSon->pointobj) ;
      curSon = sciGetSons ( pSubWin ) ;
    }
    else
    {
      curSon = curSon->pnext ;
    }
    
  }
}
/*--------------------------------------------------------------------------------*/
/* reinit a subwindow (but don't change position ) */
void reinitSubWin( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin) ;
  
  clearSubWin(   pSubWin ) ;
 
  initSubWinBounds( pSubWin ) ;
  ppSubWin->axes.xdir = 'd' ;
  ppSubWin->axes.ydir = 'l' ;
  
  ppSubWin->visible = TRUE;
  
  initSubWinAngles( pSubWin ) ;
  
  ppSubWin->FirstPlot = TRUE;
  

}
/*--------------------------------------------------------------------------------*/
/* reinit the viewing angles of a subwindow */
void initSubWinAngles( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->is3d     = ppAxesMdl->is3d     ;
  ppSubWin->alpha_kp = ppAxesMdl->alpha_kp ;
  ppSubWin->theta_kp = ppAxesMdl->theta_kp ;
  ppSubWin->alpha    = ppAxesMdl->alpha    ;
  ppSubWin->theta    = ppAxesMdl->theta    ;
}

/*--------------------------------------------------------------------------------*/
/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->WRect[0] = ppAxesMdl->WRect[0] ;
  ppSubWin->WRect[1] = ppAxesMdl->WRect[1] ;
  ppSubWin->WRect[2] = ppAxesMdl->WRect[2] ;
  ppSubWin->WRect[3] = ppAxesMdl->WRect[3] ;

}
/*--------------------------------------------------------------------------------*/
/* set the data_bounds of the axes to the default value */
void initSubWinBounds( sciPointObj * pSubWin )
{
  sciSubWindow * ppSubWin  = pSUBWIN_FEATURE (pSubWin ) ;
  sciSubWindow * ppAxesMdl = pSUBWIN_FEATURE (getAxesModel()) ;
  ppSubWin->FRect[0] = ppAxesMdl->FRect[0] ;
  ppSubWin->FRect[1] = ppAxesMdl->FRect[1] ;
  ppSubWin->FRect[2] = ppAxesMdl->FRect[2] ;
  ppSubWin->FRect[3] = ppAxesMdl->FRect[3] ;
  ppSubWin->FRect[4] = ppAxesMdl->FRect[4] ;
  ppSubWin->FRect[5] = ppAxesMdl->FRect[5] ;

  ppSubWin->SRect[0] = ppAxesMdl->SRect[0] ;
  ppSubWin->SRect[1] = ppAxesMdl->SRect[1] ;
  ppSubWin->SRect[2] = ppAxesMdl->SRect[2] ;
  ppSubWin->SRect[3] = ppAxesMdl->SRect[3] ;
  ppSubWin->SRect[4] = ppAxesMdl->SRect[4] ;
  ppSubWin->SRect[5] = ppAxesMdl->SRect[5] ;
}
/*--------------------------------------------------------------------------------*/
/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void )
{
  
  sciPointObj * pSubWin = sciGetCurrentSubWin() ;
  if ( !sciGetAddPlot( pSubWin ) )
  {
      /* redraw the axis */
      reinitSubWin( pSubWin ) ;
      return TRUE ;
  }
  return FALSE ;
}

/*--------------------------------------------------------------------------------*/
