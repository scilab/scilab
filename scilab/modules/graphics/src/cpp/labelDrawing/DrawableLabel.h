/*------------------------------------------------------------------------*/
/* file: DrawableLabel.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a label object         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LABEL_H_
#define _DRAWABLE_LABEL_H_


#include "../DrawableObject.h"

namespace sciGraphics
{

class DrawableLabel : public DrawableObject
{

public:

  DrawableLabel( sciPointObj * pObj ) : DrawableObject( pObj ) {}

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

} ;

}

#endif /* _DRAWABLE_LABEL_H_  */
