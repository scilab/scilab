/*------------------------------------------------------------------------*/
/* file: DrawableRectangle.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a rectangle object     */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_H_
#define _DRAWABLE_RECTANGLE_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableRectangle : public DrawableClippedObject
{

public:

  DrawableRectangle( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_H_  */
