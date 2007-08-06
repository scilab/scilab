/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerFactory.h                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory to create GraphicSynchronizer objects                   */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_SYNCHRONIZER_FACTORY_H_
#define _GRAPHIC_SYNCHRONIZER_FACTORY_H_

#include "GlobalSynchronizer.h"
#include "LocalSynchronizer.h"

namespace sciGraphics
{

class GraphicSynchronizerFactory
{

public:

  /*---------------------------------------------------------------------------*/;
  /**
  * Get the global locking of the graphic.
  * @return global synchronizer used to lock the whole graphic module
  */
  static GlobalSynchronizer * getGlobalSynchronizer( void ) ;

  /**
  * Destroy the global instance for graphic synchronizing.
  * Beware that this routine is not synchronized.
  */
  static void destroyGlobalSynchronizer( void ) ;
  /*---------------------------------------------------------------------------*/

  GraphicSynchronizerFactory( void ) {}

  virtual ~GraphicSynchronizerFactory( void ) {}

  /**
   * Create an instance of GraphicSynchronizer
   */
  LocalSynchronizer * createLocalSynchronizer( void ) ;

protected:

  /**
   * Create an instance of GlobalSynchronize.
   * But since there should be only one global synchronizer.
   * User should use the singleton methods.
   */
  GlobalSynchronizer * createGlobalSynchronizer( void ) ;

  /** global synchronizer for the graphic module */
  static GlobalSynchronizer * s_pGlobalSynchronizer;

};

}

#endif /* _GRAPHIC_SYNCHRONIZER_FACTORY_H_ */
