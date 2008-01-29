/*------------------------------------------------------------------------*/
/* file: SurfaceMarkDrawerJoGL.hxx                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing marks on the vertices of surface object        */
/*        facets                                                          */
/*------------------------------------------------------------------------*/

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

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  SurfaceMarkDrawerJavaMapper * getMarkDrawerJavaMapper(void);


};

}

#endif /* _SURFACE_MARK_DRAWER_JOGL_HXX_ */
