/*------------------------------------------------------------------------*/
/* file: DrawableSurface.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a surface object       */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SURFACE_H_
#define _DRAWABLE_SURFACE_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableSurface : public DrawableClippedObject
{

public:

  DrawableSurface( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

}

#endif /* _DRAWABLE_SURFACE_H_  */
