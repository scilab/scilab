/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing marks on the vertices of surface object facets
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SURFACE_MARK_DRAWER_JOGL_HXX_
#define _SURFACE_MARK_DRAWER_JOGL_HXX_

#include "DrawableSurface.h"
#include "DrawSurfaceStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "SurfaceMarkDrawerJavaMapper.hxx"


namespace sciGraphics
{

class SurfaceMarkDrawerJoGL : public DrawSurfaceStrategy, public DrawableObjectJoGL
{

public:

  SurfaceMarkDrawerJoGL( DrawableSurface * surface ) ;

  virtual ~SurfaceMarkDrawerJoGL(void);

  /**
   * Main algorithm
   */
  virtual void drawSurface( void );

  /**
   * Call the display list of an object
   */
  virtual void showSurface( void );

  /**
   * Draw objects using precomputed data
   * by default use show.
   */
  virtual void redrawSurface( void );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  SurfaceMarkDrawerJavaMapper * getMarkDrawerJavaMapper(void);


};

}

#endif /* _SURFACE_MARK_DRAWER_JOGL_HXX_ */

