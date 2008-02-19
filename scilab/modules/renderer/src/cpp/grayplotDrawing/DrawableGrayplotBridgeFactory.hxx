/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableGrayplotBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_
#define _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_

#include "DrawableGrayplot.h"
#include "DrawableGrayplotBridge.hxx"

namespace sciGraphics
{

class DrawableGrayplotBridgeFactory
{
public:

  DrawableGrayplotBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableGrayplotBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableGrayplotBridge * create( void ) ;

  /**
  * Set the DrawablePolyline which needs algorithms
  */
  void setDrawedGrayplot( DrawableGrayplot * poly ) ;

protected:

  DrawableGrayplot * m_pDrawable;

};

}  

#endif /* _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_ */

