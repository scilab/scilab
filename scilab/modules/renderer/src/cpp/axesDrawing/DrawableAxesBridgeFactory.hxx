/*------------------------------------------------------------------------*/
/* file: DrawableAxesBridgeFactory.hxx                                    */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableAxesBridge                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_AXES_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_AXES_BRIDGE_FACTORY_HXX_

#include "DrawableAxesBridge.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

/**
 * Factory for DrawableAxesBridge
 */
class DrawableAxesBridgeFactory
{
public:

  DrawableAxesBridgeFactory(void) {m_pDrawable = NULL;}

  virtual ~DrawableAxesBridgeFactory(void) {}

  /**
  * Create a new drawer from the drawn object.
  */
  virtual DrawableAxesBridge * create( void ) ;

  /**
  * Set the DrawableRectangle which needs algorithms
  */
  void setDrawedAxes( DrawableAxes * axes ) ;

protected:

  DrawableAxes * m_pDrawable;

};

}

#endif /* _DRAWABLE_AXES_BRIDGE_FACTORY_HXX_ */
