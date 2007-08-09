/*------------------------------------------------------------------------*/
/* file: DrawableArcBridgeFactory.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableArcBridge                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_

#include "DrawableArcBridge.h"
#include "DrawableArc.h"

namespace sciGraphics
{

/**
 * Factory for DrawableArcBridge
 */
class DrawableArcBridgeFactory
{
public:

  DrawableArcBridgeFactory(void) {m_pDrawable = NULL;}

  virtual ~DrawableArcBridgeFactory(void) {}

  /**
   * Create a new drawer from the drawed object.
  */
  virtual DrawableArcBridge * create( void ) ;

  /**
  * Set the DrawableRectangle wich needs algorithms
  */
  void setDrawedArc( DrawableArc * arc ) ;

protected:

  DrawableArc * m_pDrawable;

};

}

#endif /* _DRAWABLE_ARC_BRIDGE_FACTORY_HXX_ */
