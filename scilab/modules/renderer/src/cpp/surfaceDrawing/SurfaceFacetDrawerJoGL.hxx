/*------------------------------------------------------------------------*/
/* file: SurfaceFacetDrawerJoGL.hxx                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the facets of a surface object                 */
/*------------------------------------------------------------------------*/

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
