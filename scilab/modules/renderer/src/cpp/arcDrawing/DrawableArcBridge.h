/*------------------------------------------------------------------------*/
/* file: DrawableArcBridge.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        arc object                                                      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_BRIDGE_H_
#define _DRAWABLE_ARC_BRIDGE_H_

#include "DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableArc ;

class DrawableArcBridge : public virtual DrawableClippedObjectBridge
{
public:

  DrawableArcBridge( void ) {}

  virtual ~DrawableArcBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawableArc * getArcDrawer( void ) = 0;

protected:


};

}

#endif /* _DRAWABLE_ARC_BRIDGE_H_ */
