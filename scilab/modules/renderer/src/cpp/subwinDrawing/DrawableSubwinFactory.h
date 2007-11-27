/*------------------------------------------------------------------------*/
/* file: DrawableSubwinFactory.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for subwin objects. Automaticaly create a subwin        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_FACTORY_H_
#define _DRAWABLE_SUBWIN_FACTORY_H_

#include "../DrawableObject.h"
#include "../DrawableObjectFactory.h"
#include "ConcreteDrawableSubwin.hxx"

namespace sciGraphics
{

class DrawableSubwinFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

protected:

  void setStrategies(ConcreteDrawableSubwin * subwin);

} ;

}

#endif /* _DRAWABLE_SUBWIN_FACTORY_H_  */
