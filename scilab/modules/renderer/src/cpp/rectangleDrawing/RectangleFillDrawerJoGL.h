/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the inside of a rectangle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _RECTANGLE_FILL_DRAWER_JOGL_H_
#define _RECTANGLE_FILL_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "DrawableObjectJoGL.h"
#include "RectangleFillDrawerJavaMapper.hxx"

namespace sciGraphics
{

/**
 * Strategy drawing the inside of a rectangle 
 */
class RectangleFillDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
{
public:

  RectangleFillDrawerJoGL( DrawableRectangle * drawer ) ;
  
  virtual ~RectangleFillDrawerJoGL(void);

  /**
   * Main algorithm to fill the inside of a rectangle
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
  RectangleFillDrawerJavaMapper * getFillDrawerJavaMapper(void);


};


}

#endif /* _RECTANGLE_FILL_DRAWER_JOGL_H_ */
