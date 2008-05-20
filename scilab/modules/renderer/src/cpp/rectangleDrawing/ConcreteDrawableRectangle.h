/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing driver independent algorithms to draw a
 * rectangle handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_RECTANGLE_H_
#define _CONCRETE_DRAWABLE_RECTANGLE_H_

#include <list>

#include "DrawableRectangle.h"
#include "DrawRectangleStrategy.h"

namespace sciGraphics
{

class ConcreteDrawableRectangle : public DrawableRectangle
{

public:

  ConcreteDrawableRectangle( sciPointObj * pObj );

  virtual ~ConcreteDrawableRectangle( void );

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy( DrawRectangleStrategy * strategy );

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies( void );

  /**
   * compute the coordinates of the 4 edges of the rectangle given clockwise.
   */
  virtual void getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] ) ;

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Actually draw the arc on the screen
   */
  virtual void drawRectangle(void);

  /**
   * Redraw arc from precomputed data
   */
  virtual void redrawRectangle(void);

  /**
   * Actually show the arc stored data
   */
  virtual void showRectangle(void);
  /*----------------------------------------------------------------------*/
  std::list<DrawRectangleStrategy *> m_oDrawingStrategies;
  /*----------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_RECTANGLE_H_ */
