/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableRectangle.h                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing driver independent algorithms to draw a        */
/*        rectangle handle                                                */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_RECTANGLE_H_
#define _CONCRETE_DRAWABLE_RECTANGLE_H_

#include "DrawableRectangle.h"

namespace sciGraphics
{

class ConcreteDrawableRectangle : public DrawableRectangle
{

public:

  ConcreteDrawableRectangle( sciPointObj * pObj ) : DrawableRectangle(pObj) {}

  virtual ~ConcreteDrawableRectangle( void ) {}

  /**
   * compute the coordinates of the 4 edges of the rectangle given clockwise.
   */
  virtual void getCornersCoordinates( double corner1[3], double corner2[3], double corner3[3], double corner4[3] ) ;

};

}

#endif /* _CONCRETE_DRAWABLE_RECTANGLE_H_ */