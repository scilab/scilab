/*------------------------------------------------------------------------*/
/* file: UIMenuImpFactory.h                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Factory for UIMenuImp                                */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_BRIDGE_FACTORY_H_
#define _UIMENU_BRIDGE_FACTORY_H_


#include "UIMenu.h"

namespace sciGraphics
{

  class UIMenuBridgeFactory
  {

  public:

    UIMenuBridgeFactory( void ) { m_pDrawable = NULL ; }

    virtual ~UIMenuBridgeFactory( void ) {}

    /**
     * Create a new drawer from the drawed object.
     */
    virtual UIMenuBridge * create( void ) ;
  
    /**
     * Update the drawer of the drawed object.
     * To be used when an object was deeply modified and to avoid to create it again completely.
     */
    virtual void update( void ) ;

    /**
     * Set the UIMenu wich needs algorithms
     */
    void setDrawedUIMenu( UIMenu * uimenu ) ;

  protected:

    /**
     * Create the drawing strategies.
     * @param imp Object on which to create the strategies
     */
    void setStrategies( UIMenuBridge * imp ) ;

    UIMenu * m_pDrawable ;

  } ;

}

#endif /* _DRAWABLE_RECTANGLE_BRIDGE_FACTORY_H_  */
