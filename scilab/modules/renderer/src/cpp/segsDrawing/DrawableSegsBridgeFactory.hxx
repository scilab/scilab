/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableSegsBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SEGS_BRIDGE_FACTORY_H_
#define _DRAWABLE_SEGS_BRIDGE_FACTORY_H_

#include "DrawableSegs.h"
#include "DrawableSegsBridge.hxx"

namespace sciGraphics
{

class DrawableSegsBridgeFactory
{
public:

  DrawableSegsBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableSegsBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableSegsBridge * create( void ) ;

  /**
   * Set the DrawablePolyline which needs algorithms
   */
  void setDrawedSegs( DrawableSegs * poly ) ;

protected:

  DrawableSegs * m_pDrawable;

};

}  

#endif /* _DRAWABLE_SEGS_BRIDGE_FACTORY_H_ */

