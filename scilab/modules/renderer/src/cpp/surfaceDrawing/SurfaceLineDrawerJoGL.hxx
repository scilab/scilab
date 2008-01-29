/*------------------------------------------------------------------------*/
/* file: SurfaceLineDrawerJoGL.hxx                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing lines surrounding a surface object facets      */
/*------------------------------------------------------------------------*/

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
