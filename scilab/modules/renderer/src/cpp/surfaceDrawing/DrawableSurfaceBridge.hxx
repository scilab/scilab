/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceBridge.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        surface object                                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SURFACE_BRIDGE_HXX_
#define _DRAWABLE_SURFACE_BRIDGE_HXX_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableSurface;

class DrawableSurfaceBridge : public virtual DrawableObjectBridge
{
public:

  DrawableSurfaceBridge( void ) {}

  virtual ~DrawableSurfaceBridge( void ) {}

  /**
  * Return the drawn object
  */
  virtual DrawableSurface * getSurfaceDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_SURFACE_BRIDGE_ */
