/*------------------------------------------------------------------------*/
/* file: DrawableArc.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an arc object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_H_
#define _DRAWABLE_ARC_H_


#include "../DrawableClippedObject.h"

class DrawableArc : public DrawableClippedObject
{

public:

  DrawableArc( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_ARC_H_  */
