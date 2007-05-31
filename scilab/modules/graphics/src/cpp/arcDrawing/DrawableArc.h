/*------------------------------------------------------------------------*/
/* file: DrawableArc.h                                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an arc object          */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_H_
#define _DRAWABLE_ARC_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableArc : public DrawableClippedObject
{

public:

  DrawableArc( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}


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

#endif /* _DRAWABLE_ARC_H_  */
