/*------------------------------------------------------------------------*/
/* file: DrawableArcBridgeFactory.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableArcBridge                                   */
/*------------------------------------------------------------------------*/

#include "DrawableArcBridgeFactory.hxx"
#include "DrawableArcJoGL.hxx"
#include "DrawableArcJavaMapper.hxx"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableArcBridge * DrawableArcBridgeFactory::create( void )
{
  DrawableArcJoGL * newBridge = new DrawableArcJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableArcJavaMapper());
  return newBridge;
}
/*------------------------------------------------------------------------------------------*/
void DrawableArcBridgeFactory::setDrawedArc( DrawableArc * arc )
{
  m_pDrawable = arc;
}
/*------------------------------------------------------------------------------------------*/

}
