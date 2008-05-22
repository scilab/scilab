/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a rectangle object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_RECTANGLE_H_
#define _DRAWABLE_RECTANGLE_H_


#include "../DrawableClippedObject.h"
#include "DrawableRectangleBridge.h"

namespace sciGraphics
{

class DrawableRectangle : public DrawableClippedObject
{

public:

  DrawableRectangle( sciPointObj * pObj ) ;

  virtual ~DrawableRectangle( void ) ;

  /**
   * compute the coordinates of the 4 edges of the rectangle given clockwise.
   */
  virtual void getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] ) = 0 ;

  /**
   * Return the real type of implementation object
   */
  DrawableRectangleBridge * getRectangleImp( void ) ;

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Draw the polyline using precomputed data
   */
  virtual EDisplayStatus redraw(void);

  /**
   * Actually draw the arc on the screen
   */
  virtual void drawRectangle(void) = 0;

  /**
   * Redraw arc from precomputed data
   */
  virtual void redrawRectangle(void) = 0;

  /**
   * Actually show the arc stored data
   */
  virtual void showRectangle(void) = 0;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_RECTANGLE_H_  */
