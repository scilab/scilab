/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableRectangleImp  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_
#define _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_


#include "DrawableRectangle.h"

namespace sciGraphics
{

class DrawableRectangleBridgeFactory
{

public:

  DrawableRectangleBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableRectangleBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableRectangleBridge * create( void ) ;

  /**
   * Set the DrawableRectangle wich needs algorithms
   */
  void setDrawedRectangle( DrawableRectangle * rect ) ;

protected:

  DrawableRectangle * m_pDrawable ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_  */
