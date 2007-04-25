/*------------------------------------------------------------------------*/
/* file: DrawableArcFactory.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for arc objects. Automaticaly create an arc             */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_FACTORY_H_
#define _DRAWABLE_ARC_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableArcFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_ARC_FACTORY_H_  */
