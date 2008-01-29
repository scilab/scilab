/*------------------------------------------------------------------------*/
/* file: DrawableSurface.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a surface object       */
/*------------------------------------------------------------------------*/


#include "DrawableSurface.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
void DrawableSurface::draw( void )
{
  if ( !checkVisibility() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  drawSurface();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSurface::show( void )
{
  if ( !checkVisibility() )
  {
    return ;
  }
  initializeDrawing();
  clip();
  showSurface();
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/

}
