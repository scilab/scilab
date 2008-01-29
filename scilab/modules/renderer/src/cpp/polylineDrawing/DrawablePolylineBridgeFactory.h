/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridgeFactory.h                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawablePolylineBridge                              */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_BRIDGE_FACTORY_H_
#define _DRAWABLE_POLYLINE_BRIDGE_FACTORY_H_

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
   * Set the DrawablePolyline which needs algorithms
   */
  void setDrawedPolyline( DrawablePolyline * poly ) ;

protected:

  DrawablePolyline * m_pDrawable;

};

}  

#endif /* _DRAWABLE_POLYLINE_BRIDGE_FACTORY_H_ */
