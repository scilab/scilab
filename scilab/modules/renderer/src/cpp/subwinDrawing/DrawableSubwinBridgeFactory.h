/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableSubwinBridge 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_
#define _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_


#include "DrawableSubwin.h"
#include "DrawableSubwinBridge.h"

namespace sciGraphics
{

class DrawableSubwinBridgeFactory
{

public:

  DrawableSubwinBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableSubwinBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableSubwinBridge * create( void ) ;

  /**
  * Set the DrawableFigure wich needs algorithms
  */
  void setDrawedSubwin( DrawableSubwin * subwin ) ;

protected:

  DrawableSubwin * m_pDrawable ;

} ;

}

#endif /* _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_  */
