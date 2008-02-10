/*------------------------------------------------------------------------*/
/* file: DrawableAxesFactory.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for axes objects. Automaticaly create a axes            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_AXES_FACTORY_H_
#define _DRAWABLE_AXES_FACTORY_H_


#include "../DrawableObjectFactory.h"
#include "ConcreteDrawableAxes.hxx"

namespace sciGraphics
{

class DrawableAxesFactory : public DrawableObjectFactory
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

  /**
   * Create the drawing strategies.
   * @param axes Object on which to create the strategies
   */
  void setStrategies( ConcreteDrawableAxes * axes ) ;

} ;

}

#endif /* _DRAWABLE_AXES_FACTORY_H_  */
