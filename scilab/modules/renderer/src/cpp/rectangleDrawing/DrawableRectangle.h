/*------------------------------------------------------------------------*/
/* file: DrawableRectangle.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a rectangle object     */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_H_
#define _DRAWABLE_RECTANGLE_H_


#include "../DrawableClippedObject.h"
#include "DrawableRectangleImp.h"

namespace sciGraphics
{

class DrawableRectangle : public DrawableClippedObject
{

public:

  DrawableRectangle( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  virtual ~DrawableRectangle( void ) ;

  /**
   * compute the coordinates of the 4 edges of the rectangle given clockwise.
   */
  void getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] ) ;

  /**
   * Return the real type of implementation object
   */
  DrawableRectangleImp * getRectangleImp( void ) ;

protected:

  /*---------------------------------------------------------------------------------*/
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
   * Actually draw the rectangle on the screen
   */
  void drawRectangle( void ) ;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_RECTANGLE_H_  */
