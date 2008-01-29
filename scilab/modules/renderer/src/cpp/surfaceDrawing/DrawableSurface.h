/*------------------------------------------------------------------------*/
/* file: DrawableSurface.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a surface object       */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SURFACE_H_
#define _DRAWABLE_SURFACE_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableSurface : public DrawableClippedObject
{

public:

  DrawableSurface( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

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
   * Actually draw the surface on the screen
   */
  virtual void drawSurface(void) = 0;

  /**
   * Actually show the stored data.
   */
  virtual void showSurface(void) = 0;


} ;

}

#endif /* _DRAWABLE_SURFACE_H_  */
