/*------------------------------------------------------------------------*/
/* file: drawRectangleEntity.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Rectangle handle            */
/*------------------------------------------------------------------------*/

#include "drawRectangleEntity.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "clipping.h"
#include "drawMarks.h"
#include "periScreen.h"

/*----------------------------------------------------------------------------------*/
int drawRectangleEntity( sciPointObj * pObj )
{
  sciRectangle * ppRect = pRECTANGLE_FEATURE( pObj ) ;
  sciPointObj * parentSubWin = sciGetParentSubwin(pObj) ;
  /* store the size of the rectangle */
  double ulPoint[3]  = { ppRect->x, ppRect->y, ppRect->z } ;
  double rectSize[2] = { ppRect->width, ppRect->height }   ;
  int rectEdgesX[4] ;
  int rectEdgesY[4] ;
  int lineWidth = sciGetLineWidth(pObj) ;
  int lineStyle = sciGetLineStyle(pObj) ;
  int v = 0 ;
  double dv = 0.0 ;
  int DPI[2] ;

  if (!sciGetVisibility(pObj)) { return 0 ; }
  
  GetDPIFromDriver(DPI) ;

#ifdef _MSC_VER 
  flag_DO = MaybeSetWinhdc ();
#endif

  C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);    
  C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
    PD0, PD0, PD0, 0L, 0L);

#ifdef _MSC_VER
  if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif

  /* get the 4 vertices of the rectangle in pixels */
  rectangleDouble2Pixel( parentSubWin,
                         ulPoint     ,
                         rectSize    ,
                         rectEdgesX  ,
                         rectEdgesY   ) ;

  /**DJ.Abdemouche 2003**/
  if ( !sciGetIs3d(parentSubWin) )
  {
    int rectPixPosX   ;
    int rectPixPosY   ;
    int rectPixWidth  ;
    int rectPixHeight ;

    /* retrieve x,y,w,h */
    rectPixPosX   = rectEdgesX[0] ;
    rectPixPosY   = rectEdgesY[0] ;
    rectPixWidth  = rectEdgesX[2] - rectEdgesX[0] ;
    rectPixHeight = rectEdgesY[2] - rectEdgesY[0] ;

    if ( ppRect->strwidth == 0 )
    {
      ppRect->strwidth  = rectPixWidth  ;
      ppRect->strheight = rectPixHeight ;

    }


#ifdef _MSC_VER 
    flag_DO = MaybeSetWinhdc ();
#endif
    sciClip(pObj);

    if( sciGetIsFilled(pObj) )
    {
      /*x[0] = sciGetBackground(pobj);*/
      int background = sciGetBackground(pObj) ;
      C2F (dr) ("xset", "dashes", &background, &background, &v, &v, &v, &v, 
        &dv, &dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", &background, &background, &v, &v, &v,&v, 
        &dv, &dv, &dv, &dv, 5L, 10L );
      C2F(dr)("xfrect","xv",&rectPixPosX,&rectPixPosY,&rectPixWidth,&rectPixHeight,PI0,PI0,PD0,PD0,PD0,PD0,6L,2L);
    }

    if( sciGetIsMark(pObj) )
    {
      int markForeground = sciGetMarkForeground(pObj) ;
      int markStyle = sciGetMarkStyle(pObj) ;
      int markSize = sciGetMarkSize(pObj) ;
      int xtmp[4] ;
      int ytmp[4] ;
      int nbMarks = 4 ;

      C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
        &dv, &dv, &dv, &dv, 5L, 4096);

      C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
        PD0, PD0, 0L, 0L);

      xtmp[0] = rectPixPosX ;
      xtmp[1] = rectPixPosX + rectPixWidth ;
      xtmp[2] = rectPixPosX + rectPixWidth ;
      xtmp[3] = rectPixPosX ;
      ytmp[0] = rectPixPosY ;
      ytmp[1] = rectPixPosY ;
      ytmp[2] = rectPixPosY + rectPixHeight ;
      ytmp[3] = rectPixPosY + rectPixHeight ;

      DrawNewMarks(pObj,nbMarks,xtmp,ytmp,DPI);
    }

    if ( sciGetIsLine(pObj) )
    {
      int foreground = sciGetForeground(pObj) ;

      C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
        &dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);


      if (ppRect->str == 1)
      {
        rectPixPosY -= ppRect->strheight;
        C2F(dr)("xrect","xv",&rectPixPosX,&rectPixPosY,&(ppRect->strwidth),&(ppRect->strheight),PI0,PI0,PD0,PD0,PD0,PD0,5L,2L);
      }
      else
      {
        C2F(dr)("xrect","xv",&rectPixPosX,&rectPixPosY,&rectPixWidth,&rectPixHeight,PI0,PI0,PD0,PD0,PD0,PD0,5L,2L);
      }
    }

    sciUnClip(pObj);
#ifdef _MSC_VER
    if ( flag_DO == 1) { ReleaseWinHdc() ; }
#endif
  }
  else /* Rect. in 3D */
  { 
    int close = 1 ;
    int nbVertices = 4 ;

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
      C2F (dr) ("xarea", "xv", &nbVertices, rectEdgesX, rectEdgesY, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,2L);
    }

    if ( sciGetIsMark(pObj) ) 
    {
      int markForeground = sciGetMarkForeground(pObj) ;
      int markStyle = sciGetMarkStyle(pObj) ;
      int markSize = sciGetMarkSize(pObj) ;

      C2F (dr) ("xset", "dashes", &markForeground, &markForeground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", &markForeground, &markForeground, &v, &v, &v, &v,
        &dv, &dv, &dv, &dv, 5L, 4096);

      C2F (dr) ("xset", "mark", &markStyle, &markSize, PI0, PI0, PI0, PI0, PD0, PD0,
        PD0, PD0, 0L, 0L);


      DrawNewMarks(pObj,nbVertices,rectEdgesX,rectEdgesY,DPI);
    }

    if ( sciGetIsLine(pObj) ) 
    {
      int foreground = sciGetForeground(pObj) ;
      C2F (dr) ("xset", "dashes", &foreground, &foreground, &v, &v, &v, &v, &dv,
        &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "foreground", &foreground, &foreground, &v, &v, &v, &v,
        &dv, &dv, &dv, &dv, 5L, 4096);
      C2F (dr) ("xset", "thickness", &lineWidth, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);    
      C2F (dr) ("xset", "line style", &lineStyle, PI0, PI0, PI0, PI0, PI0, PD0,
        PD0, PD0, PD0, 0L, 0L);

      C2F (dr) ("xlines", "xv", &nbVertices, rectEdgesX, rectEdgesY, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }

    sciUnClip(pObj);
#ifdef _MSC_VER 
    if ( flag_DO == 1) { ReleaseWinHdc () ; }
#endif
  }

  return 0 ;
}
/*----------------------------------------------------------------------------------*/
