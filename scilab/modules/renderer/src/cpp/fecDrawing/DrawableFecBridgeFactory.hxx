/*------------------------------------------------------------------------*/
/* file: DrawableFecBridgeFactory.hxx                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFecBridge                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_BRIDGE_FACTORY_HXX_
#define _DRAWABLE_FEC_BRIDGE_FACTORY_HXX_

#include "DrawableFec.h"
#include "DrawableFecBridge.hxx"

namespace sciGraphics
{

class DrawableFecBridgeFactory
{
public:

  DrawableFecBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableFecBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableFecBridge * create( void ) ;

  /**
  * Set the DrawableFec which needs algorithms
  */
  void setDrawedFec( DrawableFec * fec ) ;

protected:

  DrawableFec * m_pDrawable;

};

}  

#endif /* _DRAWABLE_FEC_BRIDGE_FACTORY_H_ */
