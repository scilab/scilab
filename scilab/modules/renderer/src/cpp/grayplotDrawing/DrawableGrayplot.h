/*------------------------------------------------------------------------*/
/* file: DrawableGrayplot.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a grayplot object      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_H_
#define _DRAWABLE_GRAYPLOT_H_


#include "../DrawableClippedObject.h"
#include "DrawableGrayplotBridge.hxx"

namespace sciGraphics
{

class DrawableGrayplot : public DrawableClippedObject
{

public:

  DrawableGrayplot( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  virtual ~DrawableGrayplot(void) {}

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
   * Actually draw the object
   */
  virtual void drawGrayplot(void) = 0;

  /**
   * show the object
   */ 
  virtual void showGrayplot(void) = 0;

  /**
   * Return the real type of implementation object
   */
  DrawableGrayplotBridge * getGrayplotImp( void ) ;

} ;

}

#endif /* _DRAWABLE_GRAYPLOT_H_  */
