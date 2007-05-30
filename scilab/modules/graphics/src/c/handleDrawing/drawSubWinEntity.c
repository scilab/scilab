/*------------------------------------------------------------------------*/
/* file: drawSubWinEntity.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a subwindow handle            */
/*------------------------------------------------------------------------*/

#include "drawSubWinEntity.h"
#include "Axes.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "PloEch.h"
#include "Xcall1.h"
#include "periScreen.h"
#include "DrawObjects.h"
#include "GraphicZoom.h"
#include "clipping.h"
#include "DrawingBridge.h"

/*----------------------------------------------------------------------------------------*/
int drawSubWinEntity( sciPointObj * pObj )
{
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(pObj) ;

  if ( !sciGetVisibility(pObj) ) { return 0 ; }
  
  sciSetSelectedSubWin(pObj); 

  set_scale("tttftt", ppsubwin->WRect, ppsubwin->FRect,
    NULL, ppsubwin->logflags, 
    ppsubwin->ARect); 

  if ( sciGetIs3d(pObj) ) 
  {  
    int isoflag = 0 ;
    int axesFlag = ppsubwin->axes.flag[1] ;
    double xbox[8] ;
    double ybox[8] ;
    double zbox[8] ;
    int    InsideU[4] ;
    int    InsideD[4] ;
    sciSons * curSon = NULL ;

    /* to avoid re-drawing on screen during computation of the vertices (at least)... */
    /* see zoom_box function in Plo2dEch.c*/
    if ( isZoom3dOn() )
    {
      int one = 1 ;
      C2F(dr)("xset","pixmap",&one,PI0,PI0,PI0,PI0,PI0,PD0,
        PD0,PD0,PD0,0L,0L);
      C2F (dr) ("xset","wwpc", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0,0L, 0L);
    }

    /* 3D Coordinates */ /* verifier si c'est encore utile SS */
    /*To have directly all the possible ISOVIEW Modes*/
    isoflag = ( axesFlag + 1 ) / 2 ; 

    if( ppsubwin->isoview )
    {
      if( isoflag != 2 && isoflag != 3 )
      {
        if( axesFlag == 0 || axesFlag == 2 )
        {
          /* The default isoview mode is type=4 3d isometric bounds 
          derived from the data, to similarily type=2  */
          ppsubwin->axes.flag[1] = 4 ;
        }
        else if ( axesFlag == 1 )
        {
          ppsubwin->axes.flag[1] = 3 ;
        }
      }
    }
    else {
      if ( axesFlag == 3 || axesFlag == 5 )
      {
        ppsubwin->axes.flag[1] = 1; /* computed from ebox*/
      }
      else if( axesFlag == 4 || axesFlag == 6 )
      {
        /* The default NON-isoview mode is 2 computed from data*/
        ppsubwin->axes.flag[1] = 2 ;
      }
    }

    axis_3ddraw( pObj,xbox,ybox,zbox,InsideU,InsideD); /* TEST on sciGetVisibility inside : REMOVED F.Leray 21.01.05 */
    /* because axis_3ddraw displays 3d axes BUT ALSO compute + reset the 3d scale BEFORE !! */

    displayChildren( pObj ) ;

    triedre(pObj,xbox,ybox,zbox,InsideU,InsideD);
  }
  else /* we are in 2D mode...*/
  {
    sciSons * curSon = NULL ;
    int foreground = sciGetForeground( pObj ) ;
    int lineWidth  = sciGetLineWidth( pObj ) ;
    int markStyle = sciGetMarkStyle( pObj ) ;
    int markSize  = sciGetLineWidth( pObj ) ;
    int v = 0 ;
    double dv = 0.0 ;
    char strFlag[4];

#ifdef _MSC_VER
    flag_DO=MaybeSetWinhdc();
#endif
    C2F (dr) ("xset","dashes",&foreground,&foreground,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,4096);
    C2F (dr) ("xset","foreground",&foreground,&foreground,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,4096);
    C2F (dr) ("xset","thickness",&lineWidth,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset","mark",&markStyle,&markSize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    sciUpdateScaleAngles( ppsubwin->theta, ppsubwin->alpha ) ; /* for merge objects */
    sci_update_frame_bounds_2d(pObj);


    /* Clipping only exists in 2d */
    /* the global var. Cscale has just been updated */
    /* therefore I can re-compute the clipping now (if it is "clipgrf") */

    sciClip( pObj ) ; /* to update the clip_box if needed */
    sciUnClip( pObj ) ;

    DrawAxesBackground();

    drawAxesGrid( pObj ) ;

    displayChildren( pObj ) ;

    /* F.Leray 07.12.04 */
    /* TO CORRECT the bug 1115 : Big object (grayplots) could cover axes*/
    C2F (dr) ("xset","dashes",&foreground,&foreground,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,4096);
    C2F (dr) ("xset","foreground",&foreground,&foreground,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,4096);
    C2F (dr) ("xset","thickness",&lineWidth,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F (dr) ("xset","mark",&markStyle,&markSize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    rebuild_strflag( pObj, strFlag ) ;
    axis_draw2( strFlag ) ;

    labels2D_draw( pObj ) ; /* F.Leray 08.08.05 : labels' drawing at the end */

#ifdef _MSC_VER
    if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif
    wininfo("") ;
  }
  return 0 ;
}
/*----------------------------------------------------------------------------------------*/
