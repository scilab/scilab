/*------------------------------------------------------------------------*/
/* file: DrawableRectangleImpFactory.h                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleImp                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_
#define _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_


#include "DrawableRectangle.h"

namespace sciGraphics
{

class DrawableRectangleBridgeFactory
{

public:

  DrawableRectangleBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableRectangleBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableRectangleBridge * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

  /**
   * Set the DrawableRectangle wich needs algorithms
   */
  void setDrawedRectangle( DrawableRectangle * rect ) ;

protected:

  /**
   * Create the drawing strategies.
   * @param imp Object on which to create the strategies
   */
  void setStrategies( DrawableRectangleBridge * imp ) ;

  DrawableRectangle * m_pDrawable ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_  */
