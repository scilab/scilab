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

  /**
   * Actually draw the axes object
   */
  virtual void drawAxes(void) = 0;

  /**
   * Actually show the axes object
   */
  virtual void showAxes(void) = 0;


} ;

}

#endif /* _DRAWABLE_AXES_H_  */
