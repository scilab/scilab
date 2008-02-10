/*------------------------------------------------------------------------*/
/* file: DrawableAxesBridgeFactory.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableAxesBridge                                   */
/*------------------------------------------------------------------------*/

#include "DrawableAxesBridgeFactory.hxx"
#include "DrawableAxesJoGL.hxx"
#include "DrawableAxesJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableAxesBridge * DrawableAxesBridgeFactory::create( void )
{
  DrawableAxesJoGL * newBridge = new DrawableAxesJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableAxesJavaMapper());
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableAxesBridgeFactory::setDrawedAxes( DrawableAxes * axes )
{
  m_pDrawable = axes;
}
/*---------------------------------------------------------------------------------*/

}
