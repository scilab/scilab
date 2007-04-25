/*------------------------------------------------------------------------*/
/* file: DrawableFecFactory.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for fec objects. Automaticaly create a fec              */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_FACTORY_H_
#define _DRAWABLE_FEC_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableFecFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_FEC_FACTORY_H_  */
