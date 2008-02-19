/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw a 
 * surface object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SURFACE_JOGL_HXX_
#define _DRAWABLE_SURFACE_JOGL_HXX_


#include "DrawableSurfaceBridge.hxx"
#include "DrawableSurface.h"
#include "DrawableSurfaceJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableSurfaceJoGL : public DrawableSurfaceBridge, public DrawableClippedObjectJoGL
{

public:
  DrawableSurfaceJoGL( DrawableSurface * drawer ) ;

  virtual ~DrawableSurfaceJoGL( void ) ;

  /**
  * Return the drawn object
  */
  DrawableSurface * getSurfaceDrawer( void ) ;

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  DrawableSurfaceJavaMapper * getSurfaceJavaMapper(void);

};

}

#endif /* _DRAWABLE_SURFACE_JOGL_HXX_ */

