/*------------------------------------------------------------------------*/
/* file: drawFigureEntity.h                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a figure handle               */
/*------------------------------------------------------------------------*/

#include "drawFigureEntity.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "periScreen.h" /* should be removed */
#include "DrawObjects.h"
#include "DrawingBridge.h"

/*---------------------------------------------------------------------------------------------*/
int drawFigureEntity( sciPointObj * pObj )
{
  sciFigure * ppFigure   = pFIGURE_FEATURE(pObj) ;
  sciSons   * curSon     = NULL ;
  int         curWinNum  = 0 ;
  int         verbose    = 0 ;
  int         narg       = 0 ;
  double      dv         = 0 ;
  int         background = sciGetBackground (pObj) ;

  if( !ppFigure->auto_redraw ) { return 0 ; }

  /** xclear will properly upgrade background if necessary **/
#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc() ;
#endif

  /* select the right figure for drawing to avoid displaying in a wrong window */
  C2F(dr)("xget","window",&verbose,&curWinNum,&narg,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  C2F(dr)("xset","window",&(ppFigure->number),PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);

  C2F(dr)("xset","pixmap",&(ppFigure->pixmap),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  if ( ppFigure->pixmap == 0 || GetDriverId() != 0 )
  {
    /* Change background BEFORE xclear F.Leray */
    C2F(dr)("xset","background",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); 
    C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
  }
  else
  {
    C2F (dr) ("xset","wwpc", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
  }

  C2F(dr)("xset","background",&background,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L); /* Change background F.Leray*/
  C2F(dr)("xset","alufunction",&(sciGetScilabXgc(pObj)->CurDrawFunction),PI0,PI0,PI0,
    PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc(); }
#endif

  /* STOP HERE if figure is invisible: */
  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  curSon = sciGetLastSons(pObj);
  while ( curSon != NULL )
  {
    sciDrawObj( curSon->pointobj ) ;
    curSon = curSon->pprev;
  }

  /* draw the info Message */
  C2F(dr)("xinfo",sciGetInfoMessage(pObj),PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,6L,0L);

  /* return to the current window */
  C2F(dr)("xset","window",&curWinNum,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);

  return 0 ;
}
/*---------------------------------------------------------------------------------------------*/
