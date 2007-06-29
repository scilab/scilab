/*------------------------------------------------------------------------*/
/* file: DrawablePolylineBridge.h                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object                                                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_BRIDGE_
#define _DRAWABLE_POLYLINE_BRIDGE_

#include "../DrawableObject.h"
#include "../DrawableObjectImp.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawablePolyline;

class DrawablePolylineBridge : public virtual DrawableObjectImp
{
public:

  DrawablePolylineBridge( DrawablePolyline * drawer ) ;

  virtual ~DrawablePolylineBridge( void ) ;

  /**
   * Return the drawed object
   */
  DrawablePolyline * getDrawer( void ) ;

};

}

#endif /* _DRAWABLE_POLYLINE_BRIDGE_ */
