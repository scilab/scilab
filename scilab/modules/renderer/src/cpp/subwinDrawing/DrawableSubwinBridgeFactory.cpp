/*------------------------------------------------------------------------*/
/* file: DrawableSubwinBridgeFactory.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSubwinBridge                                */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinBridgeFactory.h"
#include "DrawableSubwinJoGL.h"
#include "DrawableSubwinJavaMapper.hxx"


namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwinBridgeFactory::create( void )
{
  DrawableSubwinJoGL * newBridge =  new DrawableSubwinJoGL(m_pDrawable);
  newBridge->setJavaMapper(new DrawableSubwinJavaMapper());
  return newBridge;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinBridgeFactory::setDrawedSubwin( DrawableSubwin * subwin )
{
  m_pDrawable = subwin;
}
/*---------------------------------------------------------------------------------*/

}
