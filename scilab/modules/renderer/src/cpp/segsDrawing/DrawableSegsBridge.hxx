/*------------------------------------------------------------------------*/
/* file: DrawableSegsBridge.hxx                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        segs object                                                     */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_BRIDGE_
#define _DRAWABLE_SEGS_BRIDGE_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableSegs;

class DrawableSegsBridge : public virtual DrawableObjectBridge
{
public:

  DrawableSegsBridge( void ) {}

  virtual ~DrawableSegsBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawableSegs * getSegsDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_SEGS_BRIDGE_ */
