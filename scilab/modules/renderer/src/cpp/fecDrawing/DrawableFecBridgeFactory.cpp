/*------------------------------------------------------------------------*/
/* file: DrawableFecBridgeFactory.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFecBridge                                   */
/*------------------------------------------------------------------------*/

#include "DrawableFecBridgeFactory.hxx"
#include "DrawableFecJoGL.hxx"
#include "DrawableFecJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFecBridge * DrawableFecBridgeFactory::create( void )
{
  DrawableFecJoGL * imp = new DrawableFecJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableFecJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFecBridgeFactory::setDrawedFec( DrawableFec * fec )
{
  m_pDrawable = fec ;
}
/*---------------------------------------------------------------------------------*/

}
