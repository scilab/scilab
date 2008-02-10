/*------------------------------------------------------------------------*/
/* file: DrawableAxesBridge.h                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        Axes object                                                      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_AXES_BRIDGE_H_
#define _DRAWABLE_AXES_BRIDGE_H_

#include "DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableAxes ;

class DrawableAxesBridge : public virtual DrawableClippedObjectBridge
{
public:

  DrawableAxesBridge( void ) {}

  virtual ~DrawableAxesBridge( void ) {}

  /**
  * Return the drawn object
  */
  virtual DrawableAxes * getAxesDrawer( void ) = 0;

protected:


};

}

#endif /* _DRAWABLE_AXES_BRIDGE_H_ */
