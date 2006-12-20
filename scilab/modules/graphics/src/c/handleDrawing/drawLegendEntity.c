/*------------------------------------------------------------------------*/
/* file: drawLegendEntity.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a legend handle               */
/*------------------------------------------------------------------------*/

#include "drawLegendEntity.h"
#include "GetProperty.h"
#include "Plo2d.h"
#include "Xcall1.h"
#include "periScreen.h"

/*----------------------------------------------------------------------------------------*/
int drawLegendEntity( sciPointObj * pObj )
{
  sciLegend * ppLegend = pLEGEND_FEATURE(pObj) ;

  int verbose = 0 ;
  int thickness = 0 ;
  int narg = 1 ;
  int foreground = sciGetForeground( pObj ) ;
  int fontForeground = sciGetFontForeground( pObj ) ;
  int fontWidth = sciGetFontDeciWidth( pObj ) / 100 ;
  int fontStyle = sciGetFontStyle( pObj ) ;
  int oldForeground = 0 ;
  int oldDashStyle = 0 ;
  int v = 0 ;
  double dv = 0.0 ;
  int i ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  C2F(dr1)("xget", "dashes", &verbose, &oldDashStyle, &v, &v, &v,&v, &dv, &dv, &dv, &dv, 5L, 4096);
  C2F(dr1)("xget", "foreground", &verbose, &oldForeground, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 4096);


#ifdef _MSC_VER 
  flag_DO=MaybeSetWinhdc();
#endif
  C2F (dr1) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr1) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);

  C2F(dr)("xset","font",&fontStyle,&fontWidth,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L);


  /*permet la mise a jour des legendes correspondantes aux entites associees */
  for ( i = 0; i < ppLegend->nblegends; i++ )
  {
    sciPointObj * curObj = ppLegend->pptabofpointobj[i] ;
    if ( sciGetIsMark( curObj ) )
    {
      ppLegend->pstyle[i] = -sciGetMarkStyle( curObj );
    }
    else
    {
      ppLegend->pstyle[i] = sciGetForeground( curObj );
    }
  }
  Legends( ppLegend->pstyle, &(ppLegend->nblegends), getStrMatElement(sciGetText(pObj),0,0) ) ;


  /* restore the graphic context */

  C2F (dr1) ("xset", "dashes", &oldDashStyle, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 6L);
  C2F (dr1) ("xset", "foreground", &oldForeground, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 10L);
#ifdef _MSC_VER 
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif

  return 0 ;

}
/*----------------------------------------------------------------------------------------*/
