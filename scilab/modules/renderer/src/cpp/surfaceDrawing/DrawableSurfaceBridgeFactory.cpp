/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceBridgeFactory.h                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSurfaceBridge                               */
/*------------------------------------------------------------------------*/

#include "DrawableSurfaceBridgeFactory.hxx"
#include "DrawableSurfaceJoGL.hxx"
#include "DrawableSurfaceJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSurfaceBridge * DrawableSurfaceBridgeFactory::create( void )
{
  DrawableSurfaceJoGL * imp = new DrawableSurfaceJoGL( m_pDrawable ) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableSurfaceBridgeFactory::setDrawedSurface( DrawableSurface * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
