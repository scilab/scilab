/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the inside of a rectangle                      */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_FILL_DRAWER_JOGL_H_
#define _RECTANGLE_FILL_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "../DrawableObjectJoGL.h"
#include "DrawableRectangleJoGL.h"

namespace sciGraphics
{
  class RectangleFillDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
  {
  public:

    RectangleFillDrawerJoGL( DrawableRectangleBridge * drawer ) ;

    /**
    * Main algorithm to fill the inside of a rectangle
    */
    virtual void drawRectangle( void ) ;
  
    /**
     * Call the display list of an object
     */
    virtual void show( void ) { DrawableObjectJoGL::show(); }

  protected:

  };


}

#endif /* _RECTANGLE_FILL_DRAWER_JOGL_H_ */
