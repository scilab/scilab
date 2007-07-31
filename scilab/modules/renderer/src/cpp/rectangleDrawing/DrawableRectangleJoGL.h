/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_JOGL_H_
#define _DRAWABLE_RECTANGLE_JOGL_H_


#include "DrawableRectangleBridge.h"
#include "DrawableRectangle.h"
#include "../DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableRectangleJoGL : public DrawableRectangleBridge, public DrawableClippedObjectJoGL
{

public:

  DrawableRectangleJoGL( DrawableRectangle * drawer ) ;

  virtual ~DrawableRectangleJoGL( void ) ;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) ;

  /**
   * Return the drawn object
   */
  DrawableRectangle * getRectangleDrawer( void ) ;


protected:

  /*-------------------------------------------------------------*/
  /** destroy the rectangle entity */
  void destroyRectangle( void ) ;
  /*-------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_RECTANGLE_JOGL_H_  */

