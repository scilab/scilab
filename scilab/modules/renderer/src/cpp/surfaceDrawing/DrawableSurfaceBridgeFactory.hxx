/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableSurfaceBridge    
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_

#include "DrawableSurface.h"
#include "DrawableSurfaceBridge.hxx"

namespace sciGraphics
{

class DrawableSurfaceBridgeFactory
{
public:

  DrawableSurfaceBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableSurfaceBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableSurfaceBridge * create( void ) ;

  /**
  * Set the DrawablePolyline which needs algorithms
  */
  void setDrawedSurface( DrawableSurface * poly ) ;

protected:

  DrawableSurface * m_pDrawable;

};

}  

#endif /* _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_ */

