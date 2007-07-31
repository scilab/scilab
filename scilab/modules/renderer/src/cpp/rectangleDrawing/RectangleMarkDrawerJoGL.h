/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the marks on the corners of a rectangle        */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_MARK_DRAWER_JOGL_H_
#define _RECTANGLE_MARK_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{
  class RectangleMarkDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
  {
  public:

    RectangleMarkDrawerJoGL( DrawableRectangleBridge * drawer ) ;

    /**
     * Main algorithm to draw the marks on the rectangle corners
     */
    virtual void drawRectangle( void ) ;

    /**
    * Call the display list of an object
    */
    virtual void show( void ) { DrawableObjectJoGL::show(); }

  protected:

  };


}

#endif /* _RECTANGLE_MARK_DRAWER_JOGL_H_ */
