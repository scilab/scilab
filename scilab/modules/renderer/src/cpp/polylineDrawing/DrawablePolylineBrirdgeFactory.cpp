/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridgeFactory.h                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleBridge                             */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineBridgeFactory.h"
#include "DrawablePolylineJoGL.h"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawablePolylineBridge * DrawablePolylineBridgeFactory::create( void )
{
  DrawablePolylineJoGL * imp = new DrawablePolylineJoGL( m_pDrawable ) ;

  return imp ;
}

/*------------------------------------------------------------------------------------------*/
void DrawablePolylineBridgeFactory::setDrawedPolyline( DrawablePolyline * poly )
{
  m_pDrawable = poly ;
}
/*------------------------------------------------------------------------------------------*/

}
