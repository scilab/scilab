/*------------------------------------------------------------------------*/
/* file: DrawableCompound.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a compound object      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_COMPOUND_H_
#define _DRAWABLE_COMPOUND_H_


#include "../DrawableClippedObject.h"

class DrawableCompound : public DrawableClippedObject
{

public:

  DrawableCompound( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_COMPOUND_H_  */
