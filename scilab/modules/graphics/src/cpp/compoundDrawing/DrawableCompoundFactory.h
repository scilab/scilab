/*------------------------------------------------------------------------*/
/* file: DrawableCompoundFactory.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for compound objects. Automaticaly create a compound    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_COMPOUND_FACTORY_H_
#define _DRAWABLE_COMPOUND_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableCompoundFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_COMPOUND_FACTORY_H_  */
