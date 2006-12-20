/*----------------------------------------------------------------------------------------*/
/* FILE      : clipping.h                                                                 */
/* COPYRIGHT : INRIA 2006                                                                 */
/* AUTHORS   : Jean-Baptiste Silvy, Jean-Philipe Chancelier                               */
/* DESC.     : Management of clipping when drawing                                        */
/*----------------------------------------------------------------------------------------*/

#ifndef __SCI_CLIPPING__
#define __SCI_CLIPPING__

#include "Graphics.h"
#include "ObjectStructure.h"
#include "PloEch.h"

/*----------------------------------------------------------------------------------------*/
/*
 * @memo a structure to defined a clipping rectangle in pixels
 */
typedef struct
{
  int leftX   ; /* abscissa of the left edge of the region */
  int rightX  ; /* abscissa of the rigth edge */
  int bottomY ; /* ordinate of the bottom edge */
  int topY    ; /* ordinate of the upper edge */
}
SClipRegion ;

/*----------------------------------------------------------------------------------------*/
/* 
 * @memo use for drawing precomputing clipping when drawing polygons
 * Test a single point to be within the xleft,xright,ybot,ytop bbox.
 * Sets the returned integers 4 l.s.b. as follows:
 * bit 0 if to the left of xleft.
 * bit 1 if to the right of xright.
 * bit 2 if below of ybot.
 * bit 3 if above of ytop.
 * 0 is returned if inside.
 */
int sciClipPoint(integer x, integer y, SClipRegion * clipping ) ;

void sciClipLine( integer       x1      ,
                  integer       yy1     ,
                  integer       x2      ,
                  integer       y2      ,
                  integer     * x1n     ,
                  integer     * yy1n    ,
                  integer     * x2n     ,
                  integer     * y2n     ,
                  integer     * flag    ,
                  SClipRegion * clipping  ) ;

void sciDrawInsideSegments( integer       iib, 
                            integer       iif, 
                            integer     * vx , 
                            integer     * vy ,
                            SClipRegion * clipping ) ;

/* @memo draw the segement defined by (vx[index-1],vy[index-1]) (vx[index],vy[index]) */
void sciDrawOutsideSegment( integer index, 
                            integer *vx  ,
                            integer *vy  , 
                            SClipRegion * clipping ) ;
/* 
 * @memo returns the first (vx[.],vy[.]) point inside 
 *  xleft,xright,ybot,ytop bbox. begining at index ideb
 *  or zero if the whole polyline is out 
 */
integer sciFirstInClipRegion( integer       n   ,
                              integer       ideb,
                              integer     * vx  ,
                              integer     * vy  ,
                              SClipRegion * clipping ) ;
/* 
 *  @memo returns the first (vx[.],vy[.]) point outside
 *  xleft,xright,ybot,ytop bbox.
 *  or zero if the whole polyline is out 
 */
integer sciFirstOutClipRegion( integer       n   ,
                               integer       ideb,
                               integer     * vx  ,
                               integer     * vy  ,
                               SClipRegion * clipping ) ;

/* 
 * @memo check every segment of the polyline and draw only the part which is in the
 * clip region
 */
void C2F(clipPolyLine)( integer       n     , 
                        integer     * vx    , 
                        integer     * vy    , 
                        integer       closed,
                        SClipRegion * clipping ) ;

/*----------------------------------------------------------------------------------------*/
/*
 * @memo get the real clipping (the set one the axe or the window borders) 
 * in pixels of a graphic object
 */
void getPixelClipping( sciPointObj * pObj, SClipRegion * clipping ) ;
/*----------------------------------------------------------------------------------------*/
/**
 * Set the clipping rectangl.
 * Note that clipping is not supported at all in 3D mode for now :
 * This assertion includes axes (in 3D), surfaces and parametric curves objects.
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void sciClip ( sciPointObj * pobj ) ;

/**
 * Clear the clipping rectangle 
 * @author Djalel ABDEMOUCHE
 * 29/11/2002
 */
void sciUnClip ( sciPointObj * pobj ) ;
/*----------------------------------------------------------------------------------------*/
/**
 * use current scale to set the clipping rectangle 
 */
void frame_clip_on(  void ) ;

/**
 * disable clipping (to be called after frame_clip_on
 */
void frame_clip_off( void ) ;
/*----------------------------------------------------------------------------------------*/

#endif // __SCI_CLIPPING__
