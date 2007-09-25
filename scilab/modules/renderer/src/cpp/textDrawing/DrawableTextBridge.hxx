/*------------------------------------------------------------------------*/
/* file: DrawableTextBridge.hxx                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        text object                                                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_TEXT_BRIDGE_
#define _DRAWABLE_TEXT_BRIDGE_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableText;

class DrawableTextBridge : public virtual DrawableObjectBridge
{
public:

  DrawableTextBridge( void ) {}

  virtual ~DrawableTextBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawableText * getTextDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_TEXT_BRIDGE_ */
