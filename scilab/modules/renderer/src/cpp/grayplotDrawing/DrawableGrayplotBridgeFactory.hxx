/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotBridgeFactory.hxx                                */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableGrayplotBridge                              */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_
#define _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_

#include "DrawableGrayplot.h"
#include "DrawableGrayplotBridge.hxx"

namespace sciGraphics
{

class DrawableGrayplotBridgeFactory
{
public:

  DrawableGrayplotBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableGrayplotBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableGrayplotBridge * create( void ) ;

  /**
  * Set the DrawablePolyline which needs algorithms
  */
  void setDrawedGrayplot( DrawableGrayplot * poly ) ;

protected:

  DrawableGrayplot * m_pDrawable;

};

}  

#endif /* _DRAWABLE_GRAYPLOT_BRIDGE_FACTORY_H_ */
