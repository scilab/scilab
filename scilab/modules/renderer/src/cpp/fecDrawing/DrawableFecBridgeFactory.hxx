/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableFecBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FEC_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_FEC_BRIDGE_FACTORY_HXX_

#include "DrawableFec.h"
#include "DrawableFecBridge.hxx"

namespace sciGraphics
{

class DrawableFecBridgeFactory
{
public:

  DrawableFecBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableFecBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableFecBridge * create( void ) ;

  /**
  * Set the DrawableFec which needs algorithms
  */
  void setDrawedFec( DrawableFec * fec ) ;

protected:

  DrawableFec * m_pDrawable;

};

}  

#endif /* _DRAWABLE_FEC_BRIDGE_FACTORY_H_ */

