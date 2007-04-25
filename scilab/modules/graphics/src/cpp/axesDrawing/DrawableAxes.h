/*------------------------------------------------------------------------*/
/* file: DrawableAxes.h                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an axes object         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_AXES_H_
#define _DRAWABLE_AXES_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableAxes : public DrawableClippedObject
{

public:

  DrawableAxes( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

}

#endif /* _DRAWABLE_AXES_H_  */
