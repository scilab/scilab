/*------------------------------------------------------------------------*/
/* file: drawArcEntity.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw an Arc handle                 */
/*------------------------------------------------------------------------*/

#include "drawArcEntity.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "PloEch.h"
#include "clipping.h"
#include "periScreen.h"
#include "axesScale.h"

/*-------------------------------------------------------------------------------*/
int drawArcEntity( sciPointObj * pObj )
{
  sciArc * ppArc = pARC_FEATURE(pObj) ;
  sciPointObj * parentSubWin = sciGetParentSubwin(pObj) ;
  sciSubWindow * ppSubWin = pSUBWIN_FEATURE(parentSubWin) ;
  int verbose = 0 ;
  int v = 0 ;
  double dv = 0.0 ;
  int lineWidth = sciGetLineWidth(pObj) ;
  int lineStyle = sciGetLineStyle(pObj) ;
  int xCoord = 0 ;
  int yCoord = 0 ;
  int pixWidth = 0 ;
  int pixHeight = 0 ;
  int angle1 = 0 ;
  int angle2 = 0 ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc ();
#endif

  C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, 
    PD0, PD0, PD0, PD0, 4L, 9L);   
  C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, 
    PD0, PD0, PD0, PD0, 4L, 10L);

#ifdef _MSC_VER 
  if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif
  /**DJ.Abdemouche 2003**/
  if ( sciGetIs3d(parentSubWin) )
  {
    int one = 1 ;
    double xPos = ppArc->x ;
    double yPos = ppArc->y ;
    double zPos = ppArc->z ;

    ReverseDataFor3D( parentSubWin, &xPos, &yPos, &zPos, one ) ;

    trans3d( parentSubWin,one,&xCoord,&yCoord,&xPos,&yPos,&zPos);
  }
  else
  {
    double xPos = ppArc->x ;
    double yPos = ppArc->y ;

    if ( ppSubWin->axes.reverse[0] )
    {
      xPos = xPos + ppArc->width ;
    }

    if ( ppSubWin->axes.reverse[1] )
    {
      yPos = yPos - ppArc->height ;
    }

    xCoord = XDouble2Pixel( xPos ) ;
    yCoord = YDouble2Pixel( yPos ) ;
  }

  pixWidth  = WScale(ppArc->width ) ;
  pixHeight = HScale(ppArc->height) ;

  angle1 = round( ppArc->alphabegin ) ;
  angle2 = round( ppArc->alphaend   ) ; 

  if( ppSubWin->axes.reverse[0] && !ppSubWin->axes.reverse[1] )
  {
      angle1 = 180 * 64 - ( angle2 + angle1 ) ;
  }
  else if ( ppSubWin->axes.reverse[0] && ppSubWin->axes.reverse[1] )
  {
      angle1 = 180 * 64 + angle1 ;
  }
  else if( !ppSubWin->axes.reverse[0] && ppSubWin->axes.reverse[1] )
  {
      angle1 = 360 * 64 - ( angle2 + angle1 ) ;
  }

#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc ();
#endif
  sciClip(pObj);

  if( sciGetIsFilled(pObj) )
  {
    int background = sciGetBackground(pObj) ;
    C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, 
      &dv, &dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v,&v, 
      &dv, &dv, &dv, &dv, 5L, 10L );
    C2F (dr) ("xfarc", "xv", &xCoord, &yCoord, &pixWidth, &pixHeight, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 2L);
  }

  if( sciGetIsLine(pObj) ) 
  {
    int foreground = sciGetForeground(pObj) ;
    C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, 
      &dv, &dv, &dv, &dv, 5L, 6L);
    C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v,&v, 
      &dv, &dv, &dv, &dv, 5L, 10L );
    C2F (dr) ("xarc", "xv", &xCoord, &yCoord, &pixWidth, &pixHeight, &angle1, &angle2, PD0, PD0, PD0,PD0, 5L, 2L);
  }

  sciUnClip(pObj);
#ifdef _MSC_VER 
  if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif

  return 0 ;
}
/*-------------------------------------------------------------------------------*/