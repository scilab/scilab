/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a  
 * rectangle object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_RECTANGLE_BRIDGE_H_
#define _DRAWABLE_RECTANGLE_BRIDGE_H_

#include <list>

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"
#include "DrawRectangleStrategy.h"

namespace sciGraphics
{

/**
* To avoid cyclic dependancies.
*/
class DrawableRectangle ;

class DrawableRectangleBridge : public virtual DrawableClippedObjectBridge
{

public:

  DrawableRectangleBridge( void ) ;

  virtual ~DrawableRectangleBridge( void ) ;

  /**
   * Tell the canvas to draw itself.
   */
  virtual void drawRectangle( void ) ;

  /**
   * Tell the canvas to draw itself.
   */
  virtual void redrawRectangle( void ) ;

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy( DrawRectangleStrategy * strategy ) ;

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies( void ) ;

  /**
   * Return the drawn object
   */
  virtual DrawableRectangle * getRectangleDrawer( void ) = 0 ;

protected:


  /**
   * List of different rendering algorithms to perform
   */
  std::list<DrawRectangleStrategy *> m_oDrawingStrategies ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_BRIDGE_H_  */
