/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagemet.c                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to retrieve the currents objects               */
/*------------------------------------------------------------------------*/

#include "CurrentObjectsManagement.h"
#include "BuildObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "ObjectSelection.h"
#include "GraphicSynchronizerInterface.h"
#include "WindowList.h"
#include "InitObjects.h"

/*----------------------------------------------------------------------------------*/
/* root of the graphic hierarchy */
/* singleton, there is only one screen */
static sciPointObj * sciScreenRoot     = NULL ;
static sciPointObj * sciCurrentObject  = NULL ;
static sciPointObj * sciCurrentFigure  = NULL ;

/* there can be only one console and one screen root */
BOOL sciIsConsoleCreated = FALSE ;
BOOL sciIsScreenCreated  = FALSE ;
/*----------------------------------------------------------------------------------*/
static int cf_type = 1 ;
/*----------------------------------------------------------------------------------*/
static sciPointObj * getCurrentPointedFigure(void);
/*----------------------------------------------------------------------------------*/
int get_cf_type( void )
{
  return cf_type ;
}
/*----------------------------------------------------------------------------------*/
void set_cf_type( int val )
{
  cf_type = val ;
}
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
  sciPointObj * pSousFen = NULL;

  if( !sciHasFigures() )
  {
    /* it would mean that we have change the driver to GIF,Pos or PPM and perform a xinit F.Leray 22.07.04 */
    /* for now, no higher entities than figure */
    if ( (pfigure = ConstructFigure(NULL, NULL)) != NULL )
    {
      sciSetCurrentObj(pfigure);
      sciInitCurrentFigure(pfigure);
      if ((pSousFen = ConstructSubWin(pfigure)) != NULL) {
        sciSetCurrentObj (pSousFen);
        sciSetOriginalSubWin (pfigure, pSousFen);
        set_cf_type(1);/* current figure is a graphic one */
      }
    }
  }

  return pfigure;
}
/*----------------------------------------------------------------------------------*/
BOOL sciIsCurrentFigure(sciPointObj * pFigure)
{
  return (pFigure != getFigureModel()) && (pFigure == sciGetCurrentFigure());
}
/*----------------------------------------------------------------------------------*/
int sciInitCurrentFigure( sciPointObj * mafigure )
{
  sciCurrentFigure = mafigure ;
  set_cf_type(1);/* current figure is a graphic one */
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
long sciGetCurrentHandle( void )
{
  return sciGetHandle( sciGetCurrentObj() );
}
/*----------------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentFrame( void )
{
  return NULL ;
}
/*----------------------------------------------------------------------------------*/
int sciInitCurrentFrame( sciPointObj * pFrame ) ;
/*----------------------------------------------------------------------------------*/
int sciSetCurrentFrame(  sciPointObj * pFrame ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentWindow( void )
{
  return sciGetFirstTypedSelectedSon( sciGetCurrentWindowFrame(), SCI_WINDOW ) ;
}
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentWindowFrame( void )
{
  return sciGetFirstTypedSelectedSon( sciGetCurrentScreen(), SCI_WINDOWFRAME ) ;
}
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentScreen( void )
{
  /* the screen */
  return sciScreenRoot ;
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
sciPointObj * sciGetCurrentConsole( void )
{
  return sciGetFirstTypedSelectedSon( sciGetCurrentFrame(), SCI_CONSOLE ) ;
}
/*-----------------------------------------------------------------------------*/
