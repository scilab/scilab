/*------------------------------------------------------------------------*/
/* file: DrawableSubwinBridge.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object                                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_BRIDGE_H_
#define _DRAWABLE_SUBWIN_BRIDGE_H_

#include "../DrawableObject.h"
#include "../DrawableObjectImp.h"

namespace sciGraphics
{

  /**
  * To avoid cyclic dependances.
  */
  class DrawableSubwin;

  class DrawableSubwinBridge : public virtual DrawableObjectImp
  {

  public:

    DrawableSubwinBridge( DrawableSubwin * drawer ) ;

    virtual ~DrawableSubwinBridge( void ) {}

    /**
     * Return the drawed object
     */
    DrawableSubwin * getDrawer( void ) ;


  protected:

  } ;

}

#endif /* _DRAWABLE_SUBWIN_BRIDGE_H_  */
