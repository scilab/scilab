/*------------------------------------------------------------------------*/
/* file: UIMenuBridge.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                         */
/* desc : Class containing the driver dependant routines to draw a        */
/*        uimenu object                                                */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_BRIDGE_H_
#define _UIMENU_BRIDGE_H_

#include "../../../renderer/src/cpp/DrawableObject.h"
#include "../../../renderer/src/cpp/DrawableObjectBridge.h"
#include "DrawUIMenuStrategy.h"
#include <list>

namespace sciGraphics
{

  /**
  * To avoid cyclic dependancies.
  */
  class UIMenu ;

  class UIMenuBridge : public virtual DrawableObjectBridge
  {

  public:

    UIMenuBridge( void ) ;

    virtual ~UIMenuBridge( void ) ;

    /**
     * Tell the canvas to draw itself.
     */
    virtual void drawUIMenu( void ) ;

    /**
     * Add a rendering algorithm
     */
    void addDrawingStrategy( DrawUIMenuStrategy * strategy ) ;

    /**
     * Remove all drawing strategies.
     */
    void removeDrawingStrategies( void ) ;

    /**
     * Return the drawn object
     */
    virtual UIMenu * getUIMenuDrawer( void ) = 0 ;

  protected:


    /**
     * List of different rendering algorithms to perform
     */
    std::list<DrawUIMenuStrategy *> m_oDrawingStrategies ;

  } ;

}

#endif /* _UIMENU_BRIDGE_H_  */
