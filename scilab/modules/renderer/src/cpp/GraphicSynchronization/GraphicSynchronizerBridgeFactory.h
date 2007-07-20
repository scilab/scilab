/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerBridgeFactory.h                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory to create bridges for GraphicSynchronizer Class         */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_SYNCHRONIZER_BRIDGE_FACTORY_H_
#define _GRAPHIC_SYNCHRONIZER_BRIDGE_FACTORY_H_

#include "GraphicSynchronizerBridge.h"

namespace sciGraphics
{

class GraphicSynchronizerBridgeFactory
{

public:
  GraphicSynchronizerBridgeFactory( void ) {}

  virtual ~GraphicSynchronizerBridgeFactory( void ) {}

  /**
   * Building of the bridge
   */
  GraphicSynchronizerBridge * create( void ) ;

};

}

#endif /* _GRAPHIC_SYNCHRONIZER_BRIDGE_FACTORY_H_ */