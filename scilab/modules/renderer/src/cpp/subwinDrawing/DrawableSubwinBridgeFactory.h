/*------------------------------------------------------------------------*/
/* file: DrawableSubwinBridgeFactory.h                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableSubwinBridge                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_
#define _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_


#include "DrawableSubwin.h"
//#include "DrawableSubwinBridge.h"

namespace sciGraphics
{

  class DrawableSubwinBridgeFactory
  {

  public:

    DrawableSubwinBridgeFactory( void ) { m_pDrawable = NULL ; }

    virtual ~DrawableSubwinBridgeFactory( void ) {}

    /**
    * Create a new drawer from the drawed object.
    */
    virtual DrawableSubwinBridge * create( void ) ;

    /**
    * Set the DrawableFigure wich needs algorithms
    */
    void setDrawedSubwin( DrawableSubwin * subwin ) ;

  protected:

    DrawableSubwin * m_pDrawable ;

  } ;

}

#endif /* _DRAWABLE_SUBWIN_BRIDGE_FACTORY_H_  */
