/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory to create GraphicSynchronizer objects  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAPHIC_SYNCHRONIZER_FACTORY_H_
#define _GRAPHIC_SYNCHRONIZER_FACTORY_H_

#include "GlobalSynchronizer.h"
#include "LocalSynchronizer.h"

namespace sciGraphics
{

class GraphicSynchronizerFactory
{

public:

  /*---------------------------------------------------------------------------*/
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
