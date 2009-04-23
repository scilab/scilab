/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagemet.c                                        */
/* desc : Set of functions to retrieve the currents objects               */
/*------------------------------------------------------------------------*/

#include "CurrentObjectsManagement.h"
#include "BuildObjects.h"
#include "ObjectSelection.h"
#include "WindowList.h"
#include "HandleManagement.h"
#include "InitObjects.h"
#include "MALLOC.h"

/*----------------------------------------------------------------------------------*/
/* root of the graphic hierarchy */
/* singleton, there is only one screen */
static sciPointObj * sciCurrentObject  = NULL ;
static sciPointObj * sciCurrentFigure  = NULL ;
/*----------------------------------------------------------------------------------*/
static sciPointObj * getCurrentPointedFigure(void);
/*----------------------------------------------------------------------------------*/
static sciPointObj * getCurrentPointedFigure(void)
{
  return sciCurrentFigure;
}
/*----------------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentFigure( void )
{
  /* debug F.Leray 22.07.04 */
  sciPointObj * pfigure = getCurrentPointedFigure();

  if( !sciHasFigures() )
  {
    /* it would mean that we have change the driver to GIF,Pos or PPM and perform a xinit F.Leray 22.07.04 */
    /* for now, no higher entities than figure */
    pfigure = createFullFigure(NULL);
  }

  return pfigure;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentFigure(sciPointObj * pFigure)
{
  return (   pFigure != getFigureModel()
          && sciHasFigures()
          && (pFigure == sciGetCurrentFigure()));
}
/*----------------------------------------------------------------------------------*/
int sciInitCurrentFigure( sciPointObj * mafigure )
{
  sciCurrentFigure = mafigure ;
  return 0 ;
}
/*----------------------------------------------------------------------------------*/
int sciSetCurrentFigure ( sciPointObj * mafigure )
{
  if ( sciGetCurrentFigure() == mafigure )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitCurrentFigure( mafigure ) ;
}
/*----------------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentObj( void )
{
  return sciCurrentObject ;
}
/*----------------------------------------------------------------------------------*/
void sciSetCurrentObj( sciPointObj * pobj )
{
  sciCurrentObject = pobj ;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentObject(sciPointObj * pobj)
{
	return (pobj == sciGetCurrentObj());
}
/*----------------------------------------------------------------------------------*/
long sciGetCurrentHandle( void )
{
  return sciGetHandle( sciGetCurrentObj() );
}
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentSubWin( void )
{
  sciPointObj * currentFigure = sciGetCurrentFigure() ;
  sciPointObj * currentSubwin = NULL; 
  if ( currentFigure == NULL ) { return NULL ; }
  currentSubwin = sciGetFirstTypedSelectedSon( currentFigure, SCI_SUBWIN ) ;
  return currentSubwin;
}
/*-----------------------------------------------------------------------------*/

