/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerBridgeFactory.cpp                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory to create bridges for GraphicSynchronizer Class         */
/*------------------------------------------------------------------------*/

#include "GraphicSynchronizerBridgeFactory.h"
#include "GraphicSynchronizerJava.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
GraphicSynchronizerBridge * GraphicSynchronizerBridgeFactory::create( void )
{
  return new GraphicSynchronizerJava();
}
/*------------------------------------------------------------------------------------------*/

}
