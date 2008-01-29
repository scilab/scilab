/*------------------------------------------------------------------------*/
/* file: DrawableRectangleBridge.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object                                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_BRIDGE_H_
#define _DRAWABLE_RECTANGLE_BRIDGE_H_

#include <list>

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"
#include "DrawRectangleStrategy.h"

namespace sciGraphics
{

/**
* To avoid cyclic dependancies.
*/
class DrawableRectangle ;

class DrawableRectangleBridge : public virtual DrawableClippedObjectBridge
{

public:

  DrawableRectangleBridge( void ) ;

  virtual ~DrawableRectangleBridge( void ) ;

  /**
   * Tell the canvas to draw itself.
   */
  virtual void drawRectangle( void ) ;

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy( DrawRectangleStrategy * strategy ) ;

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies( void ) ;

  /**
   * Return the drawn object
   */
  virtual DrawableRectangle * getRectangleDrawer( void ) = 0 ;

protected:


  /**
   * List of different rendering algorithms to perform
   */
  std::list<DrawRectangleStrategy *> m_oDrawingStrategies ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_BRIDGE_H_  */
