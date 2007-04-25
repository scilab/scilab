/*------------------------------------------------------------------------*/
/* file: DrawablePolyline.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a polyline object      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_H_
#define _DRAWABLE_POLYLINE_H_


#include "../DrawableClippedObject.h"

class DrawablePolyline : public DrawableClippedObject
{

public:

  DrawablePolyline( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_POLYLINE_H_  */
