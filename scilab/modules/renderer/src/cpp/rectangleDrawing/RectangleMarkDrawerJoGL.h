/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the marks on the corners of a rectangle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _RECTANGLE_MARK_DRAWER_JOGL_H_
#define _RECTANGLE_MARK_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "../DrawableObjectJoGL.h"
#include "RectangleMarkDrawerJavaMapper.hxx"

namespace sciGraphics
{
class RectangleMarkDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
{
public:

  RectangleMarkDrawerJoGL( DrawableRectangle * drawer ) ;

  virtual ~RectangleMarkDrawerJoGL(void);

  /**
   * Main algorithm to draw the marks on the rectangle corners
   */
  virtual void drawRectangle( void ) ;

  /**
  * Call the display list of an object
  */
  virtual void show( void ) { DrawableObjectJoGL::show(); }

  /**
   * Draw the rectangle using precomputed data.
   */
  virtual void redrawRectangle( void );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  RectangleMarkDrawerJavaMapper * getMarkDrawerJavaMapper(void);

};


}

#endif /* _RECTANGLE_MARK_DRAWER_JOGL_H_ */
