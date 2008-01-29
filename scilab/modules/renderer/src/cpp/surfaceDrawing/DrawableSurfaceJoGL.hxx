/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceJoGL.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        surface object with JoGL                                        */
/*------------------------------------------------------------------------*/

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
