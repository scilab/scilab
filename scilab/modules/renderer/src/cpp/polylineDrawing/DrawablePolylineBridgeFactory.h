/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridgeFactory.h                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleBridge                             */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_BRIDGE__FACTORY_H_
#define _DRAWABLE_SUBWIN_BRIDGE__FACTORY_H_

#include "DrawablePolyline.h"

namespace sciGraphics
{

class DrawablePolylineBridgeFactory
{
public:

  DrawablePolylineBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawablePolylineBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawablePolylineBridge * create( void ) ;

  /**
   * Set the DrawableRectangle wich needs algorithms
   */
  void setDrawedPolyline( DrawablePolyline * poly ) ;

protected:

  DrawablePolyline * m_pDrawable;

};

}  

#endif /* _DRAWABLE_SUBWIN_BRIDGE__FACTORY_H_ */
