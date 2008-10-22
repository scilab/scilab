/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw a
 * polyline object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_POLYLINE_JOGL_HXX_
#define _DRAWABLE_POLYLINE_JOGL_HXX_

#include "DrawablePolylineBridge.h"
#include "DrawablePolyline.h"
#include "DrawablePolylineJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawablePolylineJoGL : public DrawablePolylineBridge, public DrawableClippedObjectJoGL
{

public:
  DrawablePolylineJoGL( DrawablePolyline * drawer ) ;

  virtual ~DrawablePolylineJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawablePolyline * getPolylineDrawer( void ) ;

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawablePolylineJavaMapper * getPolylineJavaMapper(void);

};

}

#endif /* _DRAWABLE_POLYLINE_JOGL_HXX_ */
