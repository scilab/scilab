/*------------------------------------------------------------------------*/
/* file: DrawableFec.h                                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a fec object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_H_
#define _DRAWABLE_FEC_H_


#include "../DrawableClippedObject.h"

class DrawableFec : public DrawableClippedObject
{

public:

  DrawableFec( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_FEC_H_  */
