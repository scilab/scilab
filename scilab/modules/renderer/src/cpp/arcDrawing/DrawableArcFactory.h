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
#include "DrawableArc.h"

namespace sciGraphics
{

class DrawableArcFactory : public DrawableObjectFactory
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
   * @param arc Object on which to create the strategies
   */
  void setStrategies( DrawableArc * arc ) ;

} ;

}

#endif /* _DRAWABLE_ARC_FACTORY_H_  */
