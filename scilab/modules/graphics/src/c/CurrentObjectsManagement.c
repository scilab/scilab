/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagemet.c                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to retrieve the currents objects               */
/*------------------------------------------------------------------------*/

#include "CurrentObjectsManagement.h"
#include "Xcall1.h"
#include "BuildObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "ObjectSelection.h"
#include "GraphicSynchronizerInterface.h"

/*----------------------------------------------------------------------------------*/
/* root of the graphic hierarchy */
/* singleton, there is only one screen */
static sciPointObj * sciScreenRoot     = NULL ;
static sciPointObj * sciCurrentObject  = NULL ;

/* there can be only one console and one screen root */
BOOL sciIsConsoleCreated = FALSE ;
BOOL sciIsScreenCreated  = FALSE ;

/*----------------------------------------------------------------------------------*/
static int cf_type = 1 ;
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
sciPointObj * sciGetCurrentFigure( void )
{
  /* debug F.Leray 22.07.04 */
  struct BCG * moncurScilabXgc = NULL;
  sciPointObj * pfigure = NULL;


  static sciPointObj *mafigure;
  static sciPointObj *masousfen;

  moncurScilabXgc = sciGetCurrentScilabXgc();
  if(moncurScilabXgc == (struct BCG *) NULL)
    return (sciPointObj *) NULL;

  pfigure = (sciPointObj *) (moncurScilabXgc->mafigure);

  if(pfigure == (sciPointObj *) NULL )
  {
    /* it would mean that we have change the driver to GIF,Pos or PPM and perform a xinit F.Leray 22.07.04 */
    /* for now, no higher entities than figure */
    if ((mafigure = ConstructFigure( NULL, moncurScilabXgc)) != NULL)
    {
      sciSetCurrentObj (mafigure); 
      moncurScilabXgc->mafigure = mafigure;
      if ((masousfen = ConstructSubWin (mafigure, moncurScilabXgc->CurWindow)) != NULL) {
        sciSetCurrentObj (masousfen);
        sciSetOriginalSubWin (mafigure, masousfen);
        set_cf_type(1);/* current figure is a graphic one */
      }
    }
  }

  return sciGetCurrentScilabXgc()->mafigure  ;
}
/*----------------------------------------------------------------------------------*/
int sciInitCurrentFigure( sciPointObj * mafigure )
{
  sciGetCurrentScilabXgc()->mafigure = mafigure ;
  set_cf_type(1);/* current figure is a graphic one */
  return 0 ;
}
/*----------------------------------------------------------------------------------*/
int sciSetCurrentFigure ( sciPointObj * mafigure )
{
  if ( sciGetCurPointedFigure() == mafigure )
  {
    /* nothing to do */
    return 1 ;
  }
  return sciInitCurrentFigure( mafigure ) ;
}
/*----------------------------------------------------------------------------------*/
sciPointObj * sciGetCurPointedFigure( void )
{
  if ( sciGetCurrentScilabXgc() != NULL )
  {
    return sciGetCurrentScilabXgc()->mafigure ;
  }
  else
  {
    return NULL ;
  }
}
/*----------------------------------------------------------------------------------*/
struct BCG * sciGetCurrentScilabXgc( void )
{
  int verbose = 0 ;
  double * XGC = NULL ;

  C2F(dr)("xget","gc",&verbose,PI0,PI0,PI0,PI0,PI0,(double*)&XGC,PD0,PD0,PD0,5L,10L) ;
  return (struct BCG *) XGC;
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
  if ( currentFigure == NULL ) { return NULL ; }
  return sciGetFirstTypedSelectedSon( currentFigure, SCI_SUBWIN ) ;
}
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentConsole( void )
{
  return sciGetFirstTypedSelectedSon( sciGetCurrentFrame(), SCI_CONSOLE ) ;
}
/*-----------------------------------------------------------------------------*/
