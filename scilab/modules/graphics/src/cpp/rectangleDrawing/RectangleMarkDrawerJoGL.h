/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the marks on the corners of a rectangle        */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_MARK_DRAWER_JOGL_H_
#define _RECTANGLE_MARK_DRAWER_JOGL_H_

#include "DrawRectangleStrategyJoGL.h"

namespace sciGraphics
{
  class RectangleMarkDrawerJoGL : public DrawRectangleStrategyJoGL
  {
  public:

    RectangleMarkDrawerJoGL( DrawableRectangleJoGL * drawer ) : DrawRectangleStrategyJoGL( drawer ) {}
    /**
    * Main algorithm to draw the marks on the rectangle corners
    */
    virtual void drawRectangle( void ) ;

  };


}

#endif /* _RECTANGLE_MARK_DRAWER_JOGL_H_ */
