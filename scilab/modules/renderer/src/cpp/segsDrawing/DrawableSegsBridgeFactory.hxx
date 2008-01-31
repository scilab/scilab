/*------------------------------------------------------------------------*/
/* file: DrawableSegsBridgeFactory.hxx                                    */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSegsBridge                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_BRIDGE_FACTORY_H_
#define _DRAWABLE_SEGS_BRIDGE_FACTORY_H_

#include "DrawableSegs.h"
#include "DrawableSegsBridge.hxx"

namespace sciGraphics
{

class DrawableSegsBridgeFactory
{
public:

  DrawableSegsBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableSegsBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableSegsBridge * create( void ) ;

  /**
   * Set the DrawablePolyline which needs algorithms
   */
  void setDrawedSegs( DrawableSegs * poly ) ;

protected:

  DrawableSegs * m_pDrawable;

};

}  

#endif /* _DRAWABLE_SEGS_BRIDGE_FACTORY_H_ */
