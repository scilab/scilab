/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridge.h                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object                                                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_BRIDGE_HXX_
#define _DRAWABLE_POLYLINE_BRIDGE_HXX_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawablePolyline;

class DrawablePolylineBridge : public virtual DrawableObjectBridge
{
public:

  DrawablePolylineBridge( void ) {}

  virtual ~DrawablePolylineBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawablePolyline * getPolylineDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_POLYLINE_BRIDGE_HXX_ */
