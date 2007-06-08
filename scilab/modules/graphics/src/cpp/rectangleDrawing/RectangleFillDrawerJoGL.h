/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the inside of a rectangle                      */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_FILL_DRAWER_JOGL_H_
#define _RECTANGLE_FILL_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"

namespace sciGraphics
{
  class RectangleFillDrawerJoGL : public DrawRectangleStrategy
  {
  public:

    RectangleFillDrawerJoGL( DrawableRectangleImp * drawer ) : DrawRectangleStrategy( drawer ) {}

    /**
    * Main algorithm to fill the inside of a rectangle
    */
    virtual void drawRectangle( void ) ;

  };


}

#endif /* _RECTANGLE_FILL_DRAWER_JOGL_H_ */
