/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a
 * rectangle object with JoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_RECTANGLE_JOGL_H_
#define _DRAWABLE_RECTANGLE_JOGL_H_

#include <list>

#include "DrawableRectangleBridge.h"
#include "DrawableRectangle.h"
#include "DrawableRectangleJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableRectangleJoGL : public DrawableRectangleBridge, public DrawableClippedObjectJoGL
{

public:

  DrawableRectangleJoGL( DrawableRectangle * drawer ) ;

  virtual ~DrawableRectangleJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawableRectangle * getRectangleDrawer( void ) ;


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableRectangleJavaMapper * getRectangleJavaMapper(void);
  /*-------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_RECTANGLE_JOGL_H_  */

