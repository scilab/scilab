/*------------------------------------------------------------------------*/
/* file: DrawableSubwinFactory.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for subwin objects. Automaticaly create a subwin        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_FACTORY_H_
#define _DRAWABLE_SUBWIN_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableSubwinFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_SUBWIN_FACTORY_H_  */
