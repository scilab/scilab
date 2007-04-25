/*------------------------------------------------------------------------*/
/* file: DrawableText.h                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a text object          */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_TEXT_H_
#define _DRAWABLE_TEXT_H_


#include "../DrawableClippedObject.h"

class DrawableText : public DrawableClippedObject
{

public:

  DrawableText( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_TEXT_H_  */
