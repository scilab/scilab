/*------------------------------------------------------------------------*/
/* file: DrawableSubwinBridgeFactory.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSubwinBridge                                */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinBridgeFactory.h"
#include "DrawableSubwinJoGL.h"


namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwinBridgeFactory::create( void )
{
  return new DrawableSubwinJoGL(m_pDrawable);
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwinBridgeFactory::setDrawedSubwin( DrawableSubwin * subwin )
{
  m_pDrawable = subwin;
}
/*------------------------------------------------------------------------------------------*/

}
