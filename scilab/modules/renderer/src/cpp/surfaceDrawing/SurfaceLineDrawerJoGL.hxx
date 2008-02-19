/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing lines surrounding a surface object facets
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SURFACE_LINE_DRAWER_JOGL_HXX_
#define _SURFACE_LINE_DRAWER_JOGL_HXX_

#include "DrawableSurface.h"
#include "DrawSurfaceStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "SurfaceLineDrawerJavaMapper.hxx"


namespace sciGraphics
{

class SurfaceLineDrawerJoGL : public DrawSurfaceStrategy, public DrawableObjectJoGL
{

public:

  SurfaceLineDrawerJoGL( DrawableSurface * surface ) ;

  virtual ~SurfaceLineDrawerJoGL(void);

  /**
   * Main algorithm
   */
  virtual void drawSurface( void );

  /**
   * Call the display list of an object
   */
  virtual void showSurface( void );

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  SurfaceLineDrawerJavaMapper * getLineDrawerJavaMapper(void);


};

}

#endif /* _SURFACE_LINE_DRAWER_JOGL_HXX_ */

