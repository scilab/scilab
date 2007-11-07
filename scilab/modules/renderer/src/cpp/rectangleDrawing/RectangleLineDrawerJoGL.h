/*------------------------------------------------------------------------*/
/* file: RectangleLineDrawerJoGL.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the outline of a rectangle                     */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_LINE_DRAWER_JOGL_H_
#define _RECTANGLE_LINE_DRAWER_JOGL_H_

#include "DrawRectangleStrategy.h"
#include "DrawableRectangleJoGL.h"
#include "../DrawableObjectJoGL.h"
#include "RectangleLineDrawerJavaMapper.hxx"

namespace sciGraphics
{
class RectangleLineDrawerJoGL : public DrawRectangleStrategy, public DrawableObjectJoGL
{
public:

  RectangleLineDrawerJoGL( DrawableRectangleBridge * drawer ) ;

  virtual ~RectangleLineDrawerJoGL( void ) {}

  /**
   * Main algorithm to draw the outline of the rectangle
   */
  virtual void drawRectangle( void ) ;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) { DrawableObjectJoGL::show(); }

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  RectangleLineDrawerJavaMapper * getLineDrawerJavaMapper(void);


};


}

#endif /* _RECTANGLE_LINE_DRAWER_JOGL_H_ */
