/*------------------------------------------------------------------------*/
/* file: DrawableLabel.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a label object         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LABEL_H_
#define _DRAWABLE_LABEL_H_


#include "../DrawableObject.h"

class DrawableLabel : public DrawableObject
{

public:

  DrawableLabel( sciPointObj * pObj ) : DrawableObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_LABEL_H_  */
