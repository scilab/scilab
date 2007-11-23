/*------------------------------------------------------------------------*/
/* file: DrawablePolyline.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a polyline object      */
/*------------------------------------------------------------------------*/

#include "DrawablePolyline.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawablePolylineBridge * DrawablePolyline::getPolylineImp( void )
{
  return dynamic_cast<DrawablePolylineBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/
void DrawablePolyline::draw( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  drawPolyline() ;
  unClip();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawablePolyline::show( void )
{
  clip();
  showPolyline();
  unClip();
}
/*---------------------------------------------------------------------------------*/

}
