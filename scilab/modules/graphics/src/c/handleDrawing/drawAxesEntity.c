/*------------------------------------------------------------------------*/
/* file: drawAxesEntity.c                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Axes handle                 */
/*------------------------------------------------------------------------*/

#include "drawAxesEntity.h"
#include "GetProperty.h"
#include "periScreen.h"
#include "clipping.h"
#include "Xcall1.h"
#include "Axes.h"

/*-----------------------------------------------------------------------------------------*/
int drawAxesEntity( sciPointObj * pObj )
{
  sciAxes * ppAxes = pAXES_FEATURE(pObj) ;
  int foreground = sciGetForeground( pObj ) ;
  int lineWidth  = sciGetLineWidth(pObj) ;
  int v = 0 ;
  double dv = 0.0 ;
  int fontStyle = 0 ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  /* load the object foreground and dashes color */
#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc ();
#endif
  C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
    &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
    &dv, &dv, &dv, &dv, 5L, 4096);
  C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);
  sciClip(pObj);

  /* Prototype Sci_Axis HAS CHANGED:  ************* F.Leray 19.05.04
  void Sci_Axis(pos,xy_type,x,nx,y,ny,str,subtics,format,fontsize,textcolor,fontstyle,ticscolor,logflag,seg_flag, axisbuild_flag)
  For the moment, for a simple axes ( coming from a scilab command as 'drawaxis'), we set the fontstyle to 0.
  */


  Sci_Axis(ppAxes->dir,ppAxes->tics,ppAxes->vx,
    &ppAxes->nx,ppAxes->vy,&ppAxes->ny,
    ppAxes->str,ppAxes->subint,ppAxes->format,
    ppAxes->fontsize,ppAxes->textcolor,fontStyle, /* F.Leray 08.04.04 : Adding here fontstyle_zero*/
    ppAxes->ticscolor,ppAxes->logscale,ppAxes->seg, 1 ); 
#ifdef _MSC_VER 
  if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif
  sciUnClip(pObj);   

  return 0 ;
}
/*-----------------------------------------------------------------------------------------*/
