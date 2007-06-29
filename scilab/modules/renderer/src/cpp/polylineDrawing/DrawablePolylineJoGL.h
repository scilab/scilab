/*------------------------------------------------------------------------*/
/* file: DrawablePolylineJoGL.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        polyline object with JoGL                                       */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineBridge.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class DrawablePolylineJoGL : public DrawablePolylineBridge, public DrawableObjectJoGL
{

public:
  DrawablePolylineJoGL( DrawablePolyline * drawer ) ;

  virtual ~DrawablePolylineJoGL( void ) ;

};

}
