/*------------------------------------------------------------------------*/
/* file: DrawableSegs.h                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a segs object          */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_H_
#define _DRAWABLE_SEGS_H_


#include "../DrawableClippedObject.h"

class DrawableSegs : public DrawableClippedObject
{

public:

  DrawableSegs( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_SEGS_H_  */
