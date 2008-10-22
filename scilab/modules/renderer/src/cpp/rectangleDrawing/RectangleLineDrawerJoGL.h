/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the outline of a rectangle 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _RECTANGLE_LINE_DRAWER_JOGL_H_
#define _RECTANGLE_LINE_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "DrawableRectangleJoGL.h"
#include "DrawableObjectJoGL.h"
#include "RectangleLineDrawerJavaMapper.hxx"

namespace sciGraphics
{
class RectangleLineDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
{
public:

  RectangleLineDrawerJoGL( DrawableRectangle * drawer ) ;

  virtual ~RectangleLineDrawerJoGL( void );

  /**
   * Main algorithm to draw the outline of the rectangle
   */
  virtual void drawRectangle( void ) ;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) { DrawableObjectJoGL::show(); }

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  RectangleLineDrawerJavaMapper * getLineDrawerJavaMapper(void);


};


}

#endif /* _RECTANGLE_LINE_DRAWER_JOGL_H_ */
