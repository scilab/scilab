/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceFactory.h                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for surface objects. Automaticaly create a surface      */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SURFACE_FACTORY_H_
#define _DRAWABLE_SURFACE_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableSurfaceFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_SURFACE_FACTORY_H_  */
