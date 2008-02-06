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
#include "ConcreteDrawableFec.hxx"

namespace sciGraphics
{

class DrawableFecFactory : public DrawableObjectFactory
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
   * @param fec Object on which to create the strategies
   */
  void setStrategies( ConcreteDrawableFec * fec ) ;

} ;

}

#endif /* _DRAWABLE_FEC_FACTORY_H_  */
