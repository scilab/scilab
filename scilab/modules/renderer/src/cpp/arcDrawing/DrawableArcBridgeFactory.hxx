/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableArcBridge    
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_

#include "DrawableArcBridge.h"
#include "DrawableArc.h"

namespace sciGraphics
{

/**
 * Factory for DrawableArcBridge
 */
class DrawableArcBridgeFactory
{
public:

  DrawableArcBridgeFactory(void) {m_pDrawable = NULL;}

  virtual ~DrawableArcBridgeFactory(void) {}

  /**
   * Create a new drawer from the drawed object.
  */
  virtual DrawableArcBridge * create( void ) ;

  /**
  * Set the DrawableRectangle wich needs algorithms
  */
  void setDrawedArc( DrawableArc * arc ) ;

protected:

  DrawableArc * m_pDrawable;

};

}

#endif /* _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_ */

