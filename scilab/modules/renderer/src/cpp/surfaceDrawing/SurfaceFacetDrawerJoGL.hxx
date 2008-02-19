/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the facets of a surface object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SURFACE_FACET_DRAWER_JOGL_HXX_
#define _SURFACE_FACET_DRAWER_JOGL_HXX_

#include "DrawableSurface.h"
#include "DrawSurfaceStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "SurfaceFacetDrawerJavaMapper.hxx"


namespace sciGraphics
{

class SurfaceFacetDrawerJoGL : public DrawSurfaceStrategy, public DrawableObjectJoGL
{

public:

  SurfaceFacetDrawerJoGL( DrawableSurface * surface ) ;

  virtual ~SurfaceFacetDrawerJoGL(void);

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
  SurfaceFacetDrawerJavaMapper * getFacetDrawerJavaMapper(void);


};

}

#endif /* _SURFACE_FACET_DRAWER_JOGL_HXX_ */

