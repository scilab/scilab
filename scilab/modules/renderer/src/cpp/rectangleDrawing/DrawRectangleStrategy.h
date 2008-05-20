/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy interface for the drawRectangle algorithm 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_RECTANGLE_STRATEGY_H_
#define _DRAW_RECTANGLE_STRATEGY_H_

#include "DrawableRectangle.h"

namespace sciGraphics
{
class DrawRectangleStrategy
{
public:

  DrawRectangleStrategy( DrawableRectangle * drawer ) { m_pDrawed = drawer ; }
  
  virtual ~DrawRectangleStrategy( void ) {}

  /**
   * Main algorithm
   */
  virtual void drawRectangle( void ) = 0;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) = 0;

  /**
   * Draw the rectangle using precomputed data.
   * By default use show.
   */
  virtual void redrawRectangle( void ) { show(); }

protected:

  DrawableRectangle * m_pDrawed ;

};
  

}

#endif /* _DRAW_RECTANGLE_STRATEGY_H_ */
