/*------------------------------------------------------------------------*/
/* file: DrawableTextBridgeFactory.hxx                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableTextBridge                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_TEXT_BRIDGE_FACTORY_H_
#define _DRAWABLE_TEXT_BRIDGE_FACTORY_H_

#include "DrawableText.h"
#include "DrawableTextBridge.hxx"

namespace sciGraphics
{

class DrawableTextBridgeFactory
{
public:

  DrawableTextBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableTextBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableTextBridge * create( void ) ;

  /**
   * Set the DrawableText which needs algorithms
   */
  void setDrawedText( DrawableText * poly ) ;

protected:

  DrawableText * m_pDrawable;

};

}  

#endif /* _DRAWABLE_TEXT_BRIDGE_FACTORY_H_ */
