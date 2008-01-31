/*------------------------------------------------------------------------*/
/* file: DrawableSegsBridgeFactory.cpp                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleBridge                             */
/*------------------------------------------------------------------------*/

#include "DrawableSegsBridgeFactory.hxx"
#include "DrawableSegsJoGL.hxx"
#include "DrawableSegsJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSegsBridge * DrawableSegsBridgeFactory::create( void )
{
  DrawableSegsJoGL * imp = new DrawableSegsJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableSegsJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableSegsBridgeFactory::setDrawedSegs( DrawableSegs * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}
