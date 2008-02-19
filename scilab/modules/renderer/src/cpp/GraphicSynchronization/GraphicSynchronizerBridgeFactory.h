/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory to create bridges for GraphicSynchronizer Class 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
