/*------------------------------------------------------------------------*/
/* file: RectangleLinerawerJoGL.h                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the outline of a rectangle                     */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_LINE_DRAWER_JOGL_H_
#define _RECTANGLE_LINE_DRAWER_JOGL_H_

#include "DrawRectangleStrategyJoGL.h"
#include "DrawableRectangleJoGL.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{
  class RectangleLineDrawerJoGL : public DrawRectangleStrategyJoGL, public DrawableObjectJoGL
  {
  public:

    RectangleLineDrawerJoGL( DrawableRectangleJoGL * drawer ) ;

    /**
    * Main algorithm to draw the outline of the rectangle
    */
    virtual void drawRectangle( void ) ;

  };


}

#endif /* _RECTANGLE_LINE_DRAWER_JOGL_H_ */
