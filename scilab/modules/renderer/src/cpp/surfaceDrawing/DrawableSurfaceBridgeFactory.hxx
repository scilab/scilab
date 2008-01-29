/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceBridgeFactory.h                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSurfaceBridge                               */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_

#include "DrawableSurface.h"
#include "DrawableSurfaceBridge.hxx"

namespace sciGraphics
{

class DrawableSurfaceBridgeFactory
{
public:

  DrawableSurfaceBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableSurfaceBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableSurfaceBridge * create( void ) ;

  /**
  * Set the DrawablePolyline which needs algorithms
  */
  void setDrawedSurface( DrawableSurface * poly ) ;

protected:

  DrawableSurface * m_pDrawable;

};

}  

#endif /* _DRAWABLE_SURFACE_BRIDGE_FACTORY_HXX_ */
