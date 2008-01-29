/*------------------------------------------------------------------------*/
/* file: DrawablePolylineJoGL.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        polyline object with JoGL                                       */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_JOGL_HXX_
#define _DRAWABLE_POLYLINE_JOGL_HXX_

#include "DrawablePolylineBridge.h"
#include "DrawablePolyline.h"
#include "DrawablePolylineJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

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
