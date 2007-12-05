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
#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

// To avoid cyclic dependances. 
class DrawableSubwin;

class DrawableSubwinBridge : public virtual DrawableObjectBridge
{

public:

  DrawableSubwinBridge( void ) {}

  virtual ~DrawableSubwinBridge( void ) {}

protected:

  /**
   * Return the drawed object
   */
  virtual DrawableSubwin * getSubwinDrawer( void ) = 0 ;

} ;

}

#endif /* _DRAWABLE_SUBWIN_BRIDGE_H_  */
